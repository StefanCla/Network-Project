#pragma once
#include "slikenet/peerinterface.h"
#include "slikenet/MessageIdentifiers.h"
#include "Network/Network.hpp"

class BasePacketParser
{
public:
	virtual void ParsePacket(unsigned char packetID, SLNet::Packet* packet) = 0;

	//Setter
	void AddNetwork(Network* network)
	{
		m_Network = network;
	}

protected:
	Network* m_Network;

};