#pragma once
#include "slikenet/MessageIdentifiers.h"
#include "slikenet/peerinterface.h"
#include "slikenet/Gets.h"
#include "slikenet/Kbhit.h"
#include "slikenet/sleep.h"

#include <map>
#include <string>
#include <vector>
#include <Player.h>

#include <SFML/Graphics.hpp>

class Game;

class Server
{
public:
	//Constructor & Destructor
	Server(std::shared_ptr<Game> game);
	~Server();

	//Public functions
	void SetConnection();
	void Receive();

	//Public variable
	bool m_Shutdown;

private:
	//Data types needed for connection & messages
	SLNet::RakPeerInterface* m_Server;
	SLNet::Packet* m_Packet;
	SLNet::SystemAddress m_ClientID;

	unsigned char m_PacketIdentifier;
	char m_Message[MAX_CHATLENGTH];

	std::map<std::string, SLNet::SystemAddress> m_UserMap;

	//Gameplay stuff
	std::shared_ptr<Game> m_Game;

private:
	//Private functions
	unsigned char GetPacketIdentifier();
	void Whisper();
	void Circle();
	void PlayerMovement();
};