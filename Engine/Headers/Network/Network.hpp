#pragma once
#include <memory>
#include "slikenet/MessageIdentifiers.h"
#include "slikenet/peerinterface.h"
#include "slikenet/Gets.h"
#include "slikenet/Kbhit.h"
#include <unordered_map>
#include <string>

class BasePacketParser;

struct User
{
	std::string Name{ "NULL" };
	SLNet::SystemAddress SystemAddress{ SLNet::UNASSIGNED_SYSTEM_ADDRESS };
	bool Connected{ false };
};

class Network
{
public:
	Network(std::shared_ptr<BasePacketParser> packetParser);
	~Network();

	void SetConnection();
	void Receive();

	//if broadcast is true, send to all connected clients, ip will specify who not to send it to
	template <typename Packet>
	void Send(Packet* packet
		, PacketPriority packetPrio = HIGH_PRIORITY
		, PacketReliability packetRelia = RELIABLE_ORDERED
		, bool broadcast = true
		, SLNet::AddressOrGUID ip = SLNet::UNASSIGNED_SYSTEM_ADDRESS)
	{
		m_Self->Send(reinterpret_cast<const char*>(packet), sizeof(Packet) + 1, packetPrio, packetRelia, 0, ip, broadcast);
	}

	//Getters & Setters
#ifdef CLIENT
	const unsigned int GetServerPort() const;
	const unsigned int GetCurrentClients() const;
	const unsigned int GetClientID() const;
	void SetCurrentClients(unsigned int currectClients);
	void SetClientID(unsigned int clientID);
	void SetServerPort(unsigned int port);
	void SetServerIP(std::string serverIP);
#elif SERVER
	const std::unordered_map<unsigned int, User>& GetUserMap() const;
	unsigned int AddUserMapEntry(User user);
	void DisableUserMapEntry(unsigned int id);
	unsigned int GetClientIDBySystemAddress(SLNet::SystemAddress& systemAddress);
	unsigned int GetCurrentClients();
	unsigned int GetClientID();
	User& GetClientByID(unsigned int id);
#endif
	bool GetConnectingAttempt() const;
	bool GetIsConnected() const;
	void SetIsConnected(bool isConnected);

	SLNet::RakPeerInterface* m_Self;
private:
#ifdef CLIENT
	unsigned int m_CurrentClients; //Number of currently connected clients
	unsigned int m_ClientID; //Your client ID
#elif SERVER
	std::unordered_map<unsigned int, User> m_UserMap; //Client: clientID / User struct
#endif

	bool m_ConnectingAttempt; //True if the client is trying to connect to the server
	bool m_IsConnected;	//True if the client is connected with the server
	std::string m_ServerIP;
	unsigned int m_ServerPort;

	//SLNet::RakPeerInterface* m_Self;
	SLNet::Packet* m_Packet;

	unsigned char m_PacketIdentifier;
	std::shared_ptr<BasePacketParser> m_ParsePacketData;

private:
	unsigned char GetPacketIdentifier();
};