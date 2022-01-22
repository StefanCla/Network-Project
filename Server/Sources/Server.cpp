#include "Precomp.h"
#include "Server.h"
#include "CustomPackets.h"
#include "Game.h"

#include <iostream>

Server::Server(std::shared_ptr<Game> game) :
	m_Game(game),
	m_Shutdown(false)
{}

Server::~Server()
{
	//Shutdown the server correctly;
	m_Server->Shutdown(300);
	SLNet::RakPeerInterface::DestroyInstance(m_Server);
}

//Establish the server and open the listening port
void Server::SetConnection()
{
	m_Server = SLNet::RakPeerInterface::GetInstance();
	m_Server->SetIncomingPassword("RandomPass", (int)strlen("RandomPass")); //Again, since we don't send any sensetive data, we keep the password plain and simple
	m_Server->SetTimeoutTime(30000, SLNet::UNASSIGNED_SYSTEM_ADDRESS);

	m_ClientID = SLNet::UNASSIGNED_SYSTEM_ADDRESS;
	
	const int intServerPort = SERVERPORT;

	SLNet::SocketDescriptor socketDescriptors[2];
	socketDescriptors[0].port = static_cast<unsigned short>(intServerPort);
	socketDescriptors[0].socketFamily = AF_INET;
	socketDescriptors[1].port = static_cast<unsigned short>(intServerPort);
	socketDescriptors[1].socketFamily = AF_INET6;

	//Try to listen with IPv6
	bool b = m_Server->Startup(4, socketDescriptors, 2) == SLNet::RAKNET_STARTED;
	m_Server->SetMaximumIncomingConnections(4);
	
	if (!b) //If failed, try to listen with just IPv4
	{
		printf("Failed to start dual ports\n");
		b = m_Server->Startup(4, socketDescriptors, 1) == SLNet::RAKNET_STARTED;
		if (!b)
		{
			puts("Server failed to start.\n");
		}
	}
	m_Server->SetOccasionalPing(true);
	m_Server->SetUnreliableTimeout(1000);
}

//Obtain input to send as a message and receive messages from the clients
void Server::Receive()
{
	RakSleep(30);

	if (_kbhit())
	{
		Gets(m_Message, sizeof(m_Message));

		if (strcmp(m_Message, "quit") == 0)
		{
			puts("Quitting.");
			m_Shutdown = true;
		}

		char message[MAX_CHATLENGTH];
		message[0] = 0;

		//Add prefix to our message
		const static char prefixServer[] = "Server: ";
		strncpy_s(message, prefixServer, sizeof(message));
		strncat_s(message, m_Message, sizeof(message) - strlen(prefixServer) - 1);

		//Send message to all connected clients
		m_Server->Send(message, (const int)strlen(message) + 1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, SLNet::UNASSIGNED_SYSTEM_ADDRESS, true);
	}

	//Deallocate packets to correctly cast them and use their data
	for (m_Packet = m_Server->Receive(); m_Packet; m_Server->DeallocatePacket(m_Packet), m_Packet = m_Server->Receive())
	{
		m_PacketIdentifier = GetPacketIdentifier();

		switch (m_PacketIdentifier)
		{
		case ID_DISCONNECTION_NOTIFICATION:
			// Connection lost normally
			printf("ID_DISCONNECTION_NOTIFICATION from %s\n", m_Packet->systemAddress.ToString(true));;
			break;

		case ID_NEW_INCOMING_CONNECTION:
		{
			// Somebody connected.  We have their IP now
			printf("ID_NEW_INCOMING_CONNECTION from %s with GUID %s\n", m_Packet->systemAddress.ToString(true), m_Packet->guid.ToString());
			m_ClientID = m_Packet->systemAddress; // Record the player ID of the client

			printf("Remote internal IDs:\n");
			for (int index = 0; index < MAXIMUM_NUMBER_OF_INTERNAL_IDS; index++)
			{
				SLNet::SystemAddress internalId = m_Server->GetInternalID(m_Packet->systemAddress, index);
				if (internalId != SLNet::UNASSIGNED_SYSTEM_ADDRESS)
				{
					printf("%i. %s\n", index + 1, internalId.ToString(true));
				}
			}

			ClientConnectPacket* clientPacket = new ClientConnectPacket();
			clientPacket->PacketID = ID_CLIENTCONNECTED;
			clientPacket->ConnectedClients = static_cast<int>(m_UserMap.size());
			m_Server->Send((const char*)clientPacket, sizeof(ClientConnectPacket) + 1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_Packet->systemAddress, false);
			delete clientPacket;
		}
			break;

		case ID_INCOMPATIBLE_PROTOCOL_VERSION:
			printf("ID_INCOMPATIBLE_PROTOCOL_VERSION\n");
			break;

		case ID_CONNECTED_PING:
		case ID_UNCONNECTED_PING:
			printf("Ping from %s\n", m_Packet->systemAddress.ToString(true));
			break;

		case ID_CONNECTION_LOST:
			// Couldn't deliver a reliable packet - i.e. the other system was abnormally
			// terminated
			printf("ID_CONNECTION_LOST from %s\n", m_Packet->systemAddress.ToString(true));;
			break;
		case ID_NAME:
		{
			//Add their name to the map
			NamePacket* namePacket = (NamePacket*)m_Packet->data;
			m_UserMap.insert(std::pair<std::string, SLNet::SystemAddress>(namePacket->Name, m_Packet->systemAddress));
		}
			break;
		case ID_WHISPER:
			Whisper();
			break;
		case ID_CIRCLE:
			Circle();
			break;
		case ID_PLAYERMOVE:
			PlayerMovement();
			break;
		default:
			// The server knows the static data of all clients, so we can prefix the message
			// With the name data
			printf("%s\n", m_Packet->data);

			// Relay the message.  We prefix the name for other clients.  This demonstrates
			// That messages can be changed on the server before being broadcast
			// Sending is the same as before
			sprintf_s(m_Message, "%s", m_Packet->data);
			m_Server->Send(m_Message, (const int)strlen(m_Message) + 1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_Packet->systemAddress, true);
			break;
		}
	}
}

//Get packet ID
unsigned char Server::GetPacketIdentifier()
{
	if (m_Packet == 0)
		return 255;

	if ((unsigned char)m_Packet->data[0] == ID_TIMESTAMP)
	{
		RakAssert(m_Packet->length > sizeof(SLNet::MessageID) + sizeof(SLNet::Time));
		return (unsigned char)m_Packet->data[sizeof(SLNet::MessageID) + sizeof(SLNet::Time)];
	}
	else
		return (unsigned char)m_Packet->data[0];
}

//Get the whispered message
void Server::Whisper()
{
	WhisperPacket* packet = (WhisperPacket*)m_Packet->data;
	
	std::map<std::string, SLNet::SystemAddress>::iterator it;
	it = m_UserMap.find(packet->WhisperName);
	if (it != m_UserMap.end()) //Check if user is connected to the server
	{
		if (m_Packet->systemAddress == it->second) //Check if you try to send the whisper to yourself
		{}
		else
		{
			std::map<std::string, SLNet::SystemAddress>::iterator second;
			for (second = m_UserMap.begin(); second != m_UserMap.end(); second++)
			{
				if (second->second == m_Packet->systemAddress)
				{
					char message[MAX_CHATLENGTH];

					std::string temp = "From " + second->first + ": ";
					strncpy_s(message, temp.c_str(), sizeof(message));
					strncat_s(message, packet->Message, sizeof(message) - strlen(temp.c_str()) - 1);

					m_Server->Send(message, (const int)strlen(message) + 1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, it->second, false);
				}
			}
		}
	}
	else
	{
		char offline[MAX_CHATLENGTH];
		strcpy_s(offline, sizeof(offline), "Server: User is offline");
		m_Server->Send(offline, (const int)strlen(offline) + 1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_Packet->systemAddress, false);
	}
}

//Circle data | Receive circle data and send this to other clients, as well as the current circle data back
void Server::Circle()
{
	CirclePacket* circlePacket = (CirclePacket*)m_Packet->data;

	m_Game->SetPlayer(circlePacket->Diameter, circlePacket->Position, circlePacket->Color);

	//Notice all other clients a new user has connected
	m_Server->Send((const char*)circlePacket, sizeof(CirclePacket) + 1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_Packet->systemAddress, true);

	//Set all existing users to the newly connected user
	for (unsigned int i = 0; i < m_Game->GetPlayers().size() - 1; i++)
	{
		CirclePacket* circleToSend = new CirclePacket();
		circleToSend->PacketID = ID_CIRCLE;
		circleToSend->Diameter = m_Game->GetPlayers()[i]->GetDiameter();
		circleToSend->Position = m_Game->GetPlayers()[i]->GetPosition();
		circleToSend->Color = m_Game->GetPlayers()[i]->GetColor();

		m_Server->Send((const char*)circleToSend, sizeof(CirclePacket) + 1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_Packet->systemAddress, false);
		delete circleToSend;
	}
}

//Set the new position of the player
void Server::PlayerMovement()
{
	PlayerMove* movePacket = (PlayerMove*)m_Packet->data;
	m_Game->GetPlayers()[movePacket->PlayerID]->SetPosition(movePacket->Position);

	m_Server->Send((const char*)movePacket, sizeof(PlayerMove) + 1, LOW_PRIORITY, UNRELIABLE, 0, m_Packet->systemAddress, true);
}