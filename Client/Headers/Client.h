#pragma once
#include "slikenet/MessageIdentifiers.h"
#include "slikenet/peerinterface.h"
#include "slikenet/Gets.h"
#include "slikenet/Kbhit.h"
#include <string>
#include <vector>

//#include "slikenet/types.h"
//#include "slikenet/BitStream.h"
//#include "slikenet/PacketLogger.h"
//#include <unistd.h>
//#include "slikenet/statistics.h"
//#include <assert.h>
//#include <cstdio>
//#include <cstring>
//#include <stdlib.h>
//#include <limits>
//#include "slikenet/linux_adapter.h"
//#include "slikenet/osx_adapter.h"

class Client
{
public:
	//Constructor & Destructor
	Client();
	~Client();

	//Public functions
	void SetConnection();
	void Receive();

	void GetInput();
	void SetName();
	void DisplayChat();

	//Public variable
	std::string m_Name;

private:
	//Data types needed for connection & messages
	SLNet::RakPeerInterface* m_Client;
	SLNet::Packet* m_Packet;
	SLNet::SystemAddress m_ClientID;
	unsigned char m_PacketIdentifier;
	char m_Message[MAX_CHATLENGTH];
	SLNet::SystemAddress m_Address;
	int m_ClientListeningPort;

	std::string m_ReceivedMessage;
	std::vector<std::string> m_MessageArray;

	char m_Port[MAX_CHATLENGTH];
	char m_InputName[MAX_CHATLENGTH];

private:
	//Private functions
	unsigned char GetPacketIdentifier();
	void AddChatToMessageArray(const char* message);
	void ReceiveMessage();
};