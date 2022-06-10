#include "Precomp.hpp"
#include "Network/Network.hpp"
#include "Network/BasePacketParser.hpp"
#include <iostream>

#include "Network/CustomPackets.hpp"

Network::Network(std::shared_ptr<BasePacketParser> packetParser)
	: m_ParsePacketData(packetParser)
	, m_Self(nullptr)
	, m_ConnectingAttempt(false)
	, m_IsConnected(false)
{
	m_ParsePacketData->AddNetwork(this);

#ifdef SERVER
	User user;
	for (unsigned int i = 0; i < MAXPLAYERS; i++)
	{
		m_UserMap.insert(std::pair<unsigned int, User>(i, user));
	}
#endif
}

Network::~Network()
{
	if (m_Self != nullptr)
	{
		m_Self->Shutdown(3000);
		SLNet::RakPeerInterface::DestroyInstance(m_Self);
	}
}

void Network::SetConnection()
{
#ifdef SERVER

	//Setup port
	printf("Enter server port: \n");
	std::string port = "0";
	std::cin >> port;
	m_ServerPort = atoi(port.c_str());

	//Initialize the server
	m_Self = SLNet::RakPeerInterface::GetInstance();
	m_Self->SetIncomingPassword(PASS, (int)strlen(PASS));
	m_Self->SetTimeoutTime(30000, SLNet::UNASSIGNED_SYSTEM_ADDRESS);
	m_Self->SetMaximumIncomingConnections(MAXPLAYERS);

	SLNet::SocketDescriptor socketDescriptors[2];
	socketDescriptors[0].port = static_cast<unsigned short>(m_ServerPort);
	socketDescriptors[0].socketFamily = AF_INET;
	socketDescriptors[1].port = static_cast<unsigned short>(m_ServerPort);
	socketDescriptors[1].socketFamily = AF_INET6;

	bool validStart = m_Self->Startup(MAXPLAYERS, socketDescriptors, 1) == SLNet::RAKNET_STARTED;
	if (!validStart)
		printf("Server failed to start.\n");

	m_Self->SetOccasionalPing(true);
	m_Self->SetUnreliableTimeout(1000);

	m_IsConnected = true; //Server is always connected

#elif CLIENT

	//Initialize the client
	m_Self = SLNet::RakPeerInterface::GetInstance();
	m_Self->AllowConnectionResponseIPMigration(false);

	SLNet::SocketDescriptor socketDescriptor(0, "");
	socketDescriptor.socketFamily = AF_INET;
	m_Self->Startup(8, &socketDescriptor, 1);
	m_Self->SetOccasionalPing(true);

	m_Self->Connect(m_ServerIP.c_str(), static_cast<unsigned short>(m_ServerPort), PASS, (int)strlen(PASS));

#endif

	m_ConnectingAttempt = true;
}

void Network::Receive()
{
	for (m_Packet = m_Self->Receive(); m_Packet; m_Self->DeallocatePacket(m_Packet), m_Packet = m_Self->Receive())
	{
		m_PacketIdentifier = GetPacketIdentifier();
		m_ParsePacketData->ParsePacket(m_PacketIdentifier, m_Packet);
	}
}

unsigned char Network::GetPacketIdentifier()
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

//Getters & Setters
#ifdef CLIENT
const unsigned int Network::GetServerPort() const
{
	return m_ServerPort;
}

const unsigned int Network::GetCurrentClients() const
{
	return m_CurrentClients;
}

const unsigned int Network::GetClientID() const
{
	return m_ClientID;
}

void Network::SetCurrentClients(unsigned int currentClients)
{
	m_CurrentClients = currentClients;
}

void Network::SetClientID(unsigned int clientID)
{
	m_ClientID = clientID;
}

void Network::SetServerPort(unsigned int port)
{
	m_ServerPort = port;
}

void Network::SetServerIP(std::string serverIP)
{
	m_ServerIP = serverIP;
}

#elif SERVER
const std::unordered_map<unsigned int, User>& Network::GetUserMap() const
{
	return m_UserMap;
}

unsigned int Network::AddUserMapEntry(User user)
{
	for (unsigned int i = 0; i < m_UserMap.size(); i++)
	{
		if (m_UserMap[i].Connected == false)
		{
			m_UserMap[i] = user;
			return i;
		}
	}

	printf("No user was disconnected\n");
	return UNDEFINED;
}

void Network::DisableUserMapEntry(unsigned int id)
{
	if (m_UserMap.find(id) != m_UserMap.end())
		m_UserMap[id].Connected = false;
	else
		printf("Unable to disable user\n");
}

unsigned int Network::GetClientIDBySystemAddress(SLNet::SystemAddress& systemAddress)
{
	unsigned int index = 0;
	bool found = false;

	for (unsigned int i = 0; i < m_UserMap.size(); i++)
	{
		if (m_UserMap[i].SystemAddress != systemAddress)
			index++;
		else
		{
			found = true;
			break;
		}
	}

	if (found)
		return index;
	else
		return 9999;
}

unsigned int Network::GetCurrentClients()
{
	unsigned int currentClients = 0;
	for(unsigned int i = 0; i < m_UserMap.size(); i++)
	{
		if (m_UserMap[i].Connected == true)
			currentClients++;
	}

	return currentClients;
}

unsigned int Network::GetClientID()
{
	for (unsigned int i = 0; i < m_UserMap.size(); i++)
	{
		if (m_UserMap[i].Connected == false)
			return i;
	}

	return UNDEFINED;
}

User& Network::GetClientByID(unsigned int id)
{
	return m_UserMap[id];
}
#endif

bool Network::GetConnectingAttempt() const
{
	return m_ConnectingAttempt;
}

bool Network::GetIsConnected() const
{
	return m_IsConnected;
}

void Network::SetIsConnected(bool isConnected)
{
	m_IsConnected = isConnected;
}