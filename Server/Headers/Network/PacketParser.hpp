#pragma once
#include "Network/BasePacketParser.hpp"

class Engine;
class Game;

class PacketParser : public BasePacketParser
{
public:
	PacketParser(Engine* engine, Game* game);
	void ParsePacket(unsigned char packetID, SLNet::Packet* packet) override;

private:
	SLNet::Packet* m_Packet;
	Engine* m_Engine;
	Game* m_Game;

private:
	void ParseClientDisconnected();
	void ParseNewConnection();
	void ParseStartGame();
	void ParseName();
	void ParseChat();
	void ParseMovement();
	void ParseTest();
};