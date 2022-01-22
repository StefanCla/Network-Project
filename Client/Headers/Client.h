#pragma once
#include "slikenet/MessageIdentifiers.h"
#include "slikenet/peerinterface.h"
#include "slikenet/Gets.h"
#include "slikenet/Kbhit.h"
#include <string>
#include <vector>

//Temp gameplay data
#include <SFML/Graphics.hpp> //SFML variables

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

class Game;

class Client
{
public:
	//Constructor & Destructor
	Client(std::shared_ptr<Game> game);
	~Client();

	//Public functions
	void SetConnection();
	void Receive();

	void GetInput();
	void SetName();
	void DisplayChat();

	void SendPos();

	//Public variable
	std::string m_Name;
	unsigned int m_ClientNumber;		//Your ID on the server

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

	unsigned int m_CurrentClients;	//Number of clients conencted to the server

	//Temporary gameplay data
	sf::Color m_Color = sf::Color(100, 120, 140, 255);
	sf::Vector2f m_Pos = sf::Vector2f(10.f, 10.f);
	float m_Dia = 10.f;
	int posX = 0;
	int posY = 0;

	//Gameplay
	std::shared_ptr<Game> m_Game;

private:
	//Private functions
	unsigned char GetPacketIdentifier();
	void AddChatToMessageArray(const char* message);
	void ReceiveMessage();
	void ReceiveCircle();
	void InitConnectedClients();
	void SetClientPos();
	void DisconnectedUser();
};