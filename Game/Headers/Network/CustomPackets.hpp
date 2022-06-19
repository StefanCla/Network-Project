#pragma once
#include <iostream>
#include <glm/vec2.hpp>

//Value 0 is used to initialize default values
//Value 9999 is used to initialize default values where 0 might be valid

enum CustomPackets
{
	ID_CLIENT_DISCONNECT = 135,
	ID_NEW_CLIENT,
	ID_CURRENT_CLIENTS,
	ID_LOBBY_LEAD,
	ID_START_GAME,
	ID_NAME,
	ID_CHAT,
	ID_MOVEMENT,
	ID_TEST
};

struct ClientDisconnectPacket
{
	unsigned char PacketID = ID_CLIENT_DISCONNECT;
	unsigned int ClientID{ 9999 };
};

struct NewClientPacket
{
	unsigned char PacketID = ID_NEW_CLIENT;
	unsigned int ClientID{ 9999 };
	unsigned int MaxPlayers{ 0 };
	unsigned int MaxCrates{ 0 };
	unsigned int MaxBombs{ 0 };
	unsigned int MaxUpgrades{ 0 };
};

struct CurrentClientsPacket
{
	unsigned char PacketID = ID_CURRENT_CLIENTS;
	unsigned int CurrentClients{ 0 };
};

struct LobbyLeadPacket
{
	unsigned char PacketID = ID_LOBBY_LEAD;
	bool Lead{ false };
};

struct StartGamePacket
{
	unsigned char PacketID = ID_START_GAME;
	bool StartGame{ false };
	bool ClientsConnected[MAXPLAYERS]{ false };
};

struct NamePacket
{
	unsigned char PacketID = ID_NAME;
	char Name[MAXCHATLENGTH];
};

struct ChatPacket
{
	unsigned char PacketID = ID_CHAT;
	char Message[MAXCHATLENGTH];
};

struct MovementPacket
{
	unsigned char PacketID = ID_MOVEMENT;
	unsigned int ClientID{ 9999 };
	int CellID{ 9999 };
	glm::vec2 Position{ glm::vec2(0.0f, 0.0f) };
};

struct TestPacket
{
	unsigned char PacketID = ID_TEST;
	int X{ 0 };
	int Y{ 0 };
	int Z{ 0 };
};