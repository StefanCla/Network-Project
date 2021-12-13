#include "Precomp.h"
#include "Client.h"
#include "Renderer.h"
#include "CustomPackets.h"
#include <iostream>

Client::Client() :
	m_ReceivedMessage(""),
	m_Name("")
{}

Client::~Client()
{
	//Shutdown the client correctly
	m_Client->Shutdown(300);
	SLNet::RakPeerInterface::DestroyInstance(m_Client);
}

//Establish a connection with the server
void Client::SetConnection()
{
	m_Client = SLNet::RakPeerInterface::GetInstance();
	m_ClientID = SLNet::UNASSIGNED_SYSTEM_ADDRESS;

	m_Client->AllowConnectionResponseIPMigration(false);
	const int intServerPort  = SERVERPORT;

	SLNet::SocketDescriptor socketDescriptor(static_cast<unsigned short>(m_ClientListeningPort), 0);
	socketDescriptor.socketFamily = AF_INET;
	m_Client->Startup(8, &socketDescriptor, 1);
	m_Client->SetOccasionalPing(true);

	//Currently connect to the local IP address
	//We don't send any sensetive data, so we keep the password plain and simple
	m_Client->Connect("127.0.0.1", static_cast<unsigned short>(intServerPort), "RandomPass", (int)strlen("RandomPass"));
}

//Obtain input to send as a message and receive messages from the server
void Client::Receive()
{

#ifdef _WIN32 
		Sleep(30);
#else
		usleep(30 * 1000);
#endif

		//Obtain input
		if (m_Message[0] != '\0')
		{
			//If '/w' are typed, it means we want to whisper to someone
			if(m_Message[0] == '/' && m_Message[1] == 'w')
			{
				WhisperPacket* whisper = new WhisperPacket();
				whisper->PacketID = ID_WHISPER;

				//Remove '/w'
				std::string message = m_Message;
				message.erase(message.begin(), message.begin() + 2);

				//Remove ' ' if a whitespace is after '/w'
				size_t pos = message.find(" ");
				if (message.empty() != true && pos == 0)
				{
					message.erase(message.begin(), message.begin() + 1);
					pos = message.find(" ");
				}

				//Make sure the message has actual data besides just the name
				if (pos != std::string::npos)
				{
					//Remove the target name
					whisper->WhisperName = message.substr(0, pos);
					message.erase(message.begin(), message.begin() + pos + 1);

					char message2[MAX_CHATLENGTH];
					message2[0] = 0;

					if (whisper->WhisperName == m_Name) //Avoid whispering to yourself
					{
						strcpy_s(message2, sizeof(message2), "Server: You can't whisper to yourself");
					}
					else //Modify the message so we can see that we sent our message
					{
						strcpy_s(whisper->Message, sizeof(whisper->Message), message.c_str());

						std::string temp = "To " + whisper->WhisperName + ": ";
						strncpy_s(message2, temp.c_str(), sizeof(message2));
						strncat_s(message2, message.c_str(), sizeof(message2) - strlen(temp.c_str()) - 1);
					}

					//Add the message to the vector that will display text
					AddChatToMessageArray(message2);

					//Send the whisper to the server
					m_Client->Send((const char*)whisper, sizeof(WhisperPacket) + 1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, SLNet::UNASSIGNED_SYSTEM_ADDRESS, true);
				}
				else //If it is an empty message besides the target name
				{
					AddChatToMessageArray("Message did not send");
				}

				delete whisper;
			}
			else //If it's a regular message
			{
				//Add prefix
				char message[MAX_CHATLENGTH];
				message[0] = 0;
				std::string temp = m_Name + ": ";

				strncpy_s(message, temp.c_str(), sizeof(message));
				strncat_s(message, m_Message, sizeof(message) - strlen(temp.c_str()) - 1);

				//Add the message to the vector thatw ill display text
				AddChatToMessageArray(message);

				//Send message to the server
				m_Client->Send(message, (int)strlen(message) + 1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, SLNet::UNASSIGNED_SYSTEM_ADDRESS, true);
			}

			m_Message[0] = '\0';
		}

		//Deallocate packets to correctly cast them and use their data
		for (m_Packet = m_Client->Receive(); m_Packet; m_Client->DeallocatePacket(m_Packet), m_Packet = m_Client->Receive())
		{
			m_PacketIdentifier = GetPacketIdentifier();

			switch (m_PacketIdentifier)
			{
			case ID_DISCONNECTION_NOTIFICATION:
				// Connection lost normally
				printf("ID_DISCONNECTION_NOTIFICATION\n");
				break;
			case ID_ALREADY_CONNECTED:
				// Connection lost normally
				printf("ID_ALREADY_CONNECTED with guid %" PRINTF_64_BIT_MODIFIER "u\n", m_Packet->guid.g);
				break;
			case ID_INCOMPATIBLE_PROTOCOL_VERSION:
				printf("ID_INCOMPATIBLE_PROTOCOL_VERSION\n");
				break;
			case ID_REMOTE_DISCONNECTION_NOTIFICATION: // Server telling the clients of another client disconnecting gracefully.  You can manually broadcast this in a peer to peer enviroment if you want.
				printf("ID_REMOTE_DISCONNECTION_NOTIFICATION\n");
				break;
			case ID_REMOTE_CONNECTION_LOST: // Server telling the clients of another client disconnecting forcefully.  You can manually broadcast this in a peer to peer enviroment if you want.
				printf("ID_REMOTE_CONNECTION_LOST\n");
				break;
			case ID_REMOTE_NEW_INCOMING_CONNECTION: // Server telling the clients of another client connecting.  You can manually broadcast this in a peer to peer enviroment if you want.
				printf("ID_REMOTE_NEW_INCOMING_CONNECTION\n");
				break;
			case ID_CONNECTION_BANNED: // Banned from this server
				printf("We are banned from this server.\n");
				break;
			case ID_CONNECTION_ATTEMPT_FAILED:
				printf("Connection attempt failed\n");
				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				// Sorry, the server is full.  I don't do anything here but
				// A real app should tell the user
				printf("ID_NO_FREE_INCOMING_CONNECTIONS\n");
				break;

			case ID_INVALID_PASSWORD:
				printf("ID_INVALID_PASSWORD\n");
				break;

			case ID_CONNECTION_LOST:
				// Couldn't deliver a reliable packet - i.e. the other system was abnormally
				// terminated
				printf("ID_CONNECTION_LOST\n");
				break;

			case ID_CONNECTION_REQUEST_ACCEPTED:
				// This tells the client they have connected
			{
				printf("ID_CONNECTION_REQUEST_ACCEPTED to %s with GUID %s\n", m_Packet->systemAddress.ToString(true), m_Packet->guid.ToString());
				printf("My external address is %s\n", m_Client->GetExternalID(m_Packet->systemAddress).ToString(true));
				m_Address = m_Packet->systemAddress;

				//Get the name of the newly connected client
				NamePacket* namePacket = new NamePacket();
				namePacket->PacketID = ID_NAME;
				namePacket->Name = m_Name;
				m_Client->Send((const char*)namePacket, sizeof(NamePacket) + 1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, SLNet::UNASSIGNED_SYSTEM_ADDRESS, true);
				delete namePacket;
			}
				break;
			case ID_CONNECTED_PING:
			case ID_UNCONNECTED_PING:
				printf("Ping from %s\n", m_Packet->systemAddress.ToString(true));
				break;
			default:
				//Display received messages
				ReceiveMessage();
				break;
			}
		}
}

//Get packet ID
unsigned char Client::GetPacketIdentifier()
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

//Get received message
void Client::ReceiveMessage()
{
	if (m_Packet != nullptr)
	{
		m_ReceivedMessage = (reinterpret_cast<char*>(m_Packet->data));

		AddChatToMessageArray(m_ReceivedMessage.c_str());
	}
}

//Loop through the chat vector to display on screen
void Client::DisplayChat()
{
	for (int i = 0; i < m_MessageArray.size(); i++)
	{
		ImGui::Text(m_MessageArray[i].c_str());
	}
}

//Get message to send
void Client::GetInput()
{
	char test[MAX_CHATLENGTH] = "";
	ImGui::InputText("", test, sizeof(test));

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
	{
		strcpy_s(m_Message, test);
	}
}

//Get client name & listening port
void Client::SetName()
{
	ImGui::Begin("Setup");
	ImGui::InputText("Set Name", m_InputName, sizeof(m_InputName));
	ImGui::InputText("Set Port", m_Port, sizeof(m_Port));
	ImGui::End();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
	{
		m_Name = m_InputName;
		m_ClientListeningPort = atoi(m_Port);
	}
}

//Add the message to the vector containing the last few messages
void Client::AddChatToMessageArray(const char* message)
{
	if (m_MessageArray.size() < MAX_CHATMESSAGES)
		m_MessageArray.push_back(message);
	else if (m_MessageArray.size() >= MAX_CHATMESSAGES)
	{
		m_MessageArray.erase(m_MessageArray.begin());
		m_MessageArray.push_back(message);
	}
}