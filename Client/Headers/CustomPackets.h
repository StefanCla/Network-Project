#pragma once
#include <string>

//Custom enums from the packets
enum CustomPackets
{
	ID_NAME = 135,
	ID_WHISPER
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