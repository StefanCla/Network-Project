#pragma once
#include <string>
#include <SFML/Graphics.hpp>

//Custom enums from the packets
enum CustomPackets
{
	ID_NAME = 135,
	ID_WHISPER,
	ID_CLIENTCONNECTED,
	ID_CIRCLE,
	ID_PLAYERMOVE,
	ID_DISCONNECTEDCLIENT
};

struct NamePacket
{
	unsigned char PacketID;
	std::string Name;
};

struct WhisperPacket
{
	unsigned char PacketID;
	std::string WhisperName;
	char Message[MAX_CHATLENGTH];
};

struct ClientConnectPacket
{
	unsigned char PacketID;
	unsigned int ConnectedClients;
};

struct CirclePacket
{
	unsigned char PacketID;
	float Diameter;
	sf::Vector2f Position;
	sf::Color Color;
};

struct MovePacket //Make Packet
{
	unsigned char PacketID;
	unsigned int PlayerID;
	sf::Vector2f Position;
};

struct DisconnectedPacket
{
	unsigned char PacketID;
	unsigned int PlayerID;
};