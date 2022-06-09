#include "Precomp.hpp"
#include "Network/CustomPackets.hpp"
#include "Network/PacketParser.hpp"
#include "Engine.hpp"
#include "Game.hpp"

//Objects
#include "Objects/TestObject.hpp"

PacketParser::PacketParser(Engine* engine, Game* game)
	: m_Engine(engine)
	, m_Game(game)
	, m_Packet(nullptr)
{}

void PacketParser::ParsePacket(unsigned char packetID, SLNet::Packet* packet)
{
	m_Packet = packet;

	switch (packetID)
	{
	//SLikeNet IDs
	case ID_CONNECTION_REQUEST_ACCEPTED:
		ParseConnectionAccepted();
		break;

	//Custom IDs
	case ID_CLIENT_DISCONNECT:
		ParseClientDisconnected();
		break;
	case ID_NEW_CLIENT:
		ParseNewClient();
		break;
	case ID_CURRENT_CLIENTS:
		ParseCurrentClients();
		break;
	case ID_LOBBY_LEAD:
		ParseLobbyLead();
		break;
	case ID_START_GAME:
		ParseStartGame();
		break;
	case ID_NAME:
		ParseName();
		break;
	case ID_CHAT:
		ParseChat();
		break;
	case ID_MOVEMENT:
		ParseMovement();
		break;
	case ID_TEST:
		ParseTest();
		break;
	}
}

void PacketParser::ParseConnectionAccepted()
{
	m_Network->SetIsConnected(true);

	//Return name packet
	NamePacket namePacket;
	namePacket.Name = m_Game->GetName();
	m_Network->Send(&namePacket, HIGH_PRIORITY, RELIABLE_ORDERED, true);
}

void PacketParser::ParseClientDisconnected()
{
	ClientDisconnectPacket* packet = (ClientDisconnectPacket*)m_Packet->data;
	m_Engine->GetObjectByIndex(packet->ClientID)->SetActive(false);

	if (packet->ClientID < m_Network->GetClientID())
	{
		for (unsigned int i = packet->ClientID; i < MAXPLAYERS; i++)
		{
			m_Engine->GetObjectByIndex(i) = m_Engine->GetObjectByIndex(i + 1);
		}
	}
}

void PacketParser::ParseNewClient()
{
	NewClientPacket* packet = (NewClientPacket*)m_Packet->data;
	m_Network->SetClientID(packet->ClientID);
	m_Game->SetMaxObjects(packet->MaxPlayers, packet->MaxCrates, packet->MaxBombs, packet->MaxUpgrades);
	
	m_Game->InititalizeGameObjects();
}

void PacketParser::ParseCurrentClients()
{
	CurrentClientsPacket* packet = (CurrentClientsPacket*)m_Packet->data;
	m_Network->SetCurrentClients(packet->CurrentClients);
}

void PacketParser::ParseLobbyLead()
{
	LobbyLeadPacket* packet = (LobbyLeadPacket*)m_Packet->data;
	m_Game->SetLobbyLead(packet->Lead);
}

void PacketParser::ParseStartGame()
{
	StartGamePacket* packet = (StartGamePacket*)m_Packet->data;

	//Enable all clients that are currently connected
	for (unsigned int i = 0; i < MAXPLAYERS; i++)
		m_Engine->GetObjectByIndex(i)->SetActive(packet->ClientsConnected[i]);

	m_Game->SetStartGame(packet->StartGame);
}

void PacketParser::ParseName()
{
	NamePacket* packet = (NamePacket*)m_Packet->data;

	printf("Name has been parsed\n");
	printf("Name: %s\n", packet->Name);
}

void PacketParser::ParseChat()
{
	ChatPacket* packet = (ChatPacket*)m_Packet->data;
	m_Game->AddChatToArray(packet->Message);
}

void PacketParser::ParseMovement()
{
	MovementPacket* packet = (MovementPacket*)m_Packet->data;

	if(m_Game->GetObjectInit())
		m_Engine->GetObjectByIndex(packet->ClientID)->GetTransform().position = packet->Position;
}

void PacketParser::ParseTest()
{
	TestPacket* packet = (TestPacket*)m_Packet->data;

	printf("Test has been parsed\n");
	printf("X: %i, Y: %i, Z: %i\n", packet->X, packet->Y, packet->Z);
}