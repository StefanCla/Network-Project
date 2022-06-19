#include "Precomp.hpp"
#include "Network/CustomPackets.hpp"
#include "Network/PacketParser.hpp"
#include "Engine.hpp"
#include "Game.hpp"

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
	case ID_DISCONNECTION_NOTIFICATION:
		ParseClientDisconnected();
		break;
	case ID_NEW_INCOMING_CONNECTION:
		ParseNewConnection();
		break;

	//Custom IDs
	case ID_START_GAME:
		ParseStartGame();
		break;
	case ID_NAME:
		ParseName();
		break;
	case ID_TEST:
		ParseTest();
		break;
	case ID_CHAT:
		ParseChat();
		break;
	case ID_MOVEMENT:
		ParseMovement();
		break;
	default:
		printf("Error! The first ID = %d\n", m_Packet->data[0]);
		break;
	}
}

void PacketParser::ParseClientDisconnected()
{
	unsigned int clientID = m_Network->GetClientIDBySystemAddress(m_Packet->systemAddress);

	m_Engine->GetObjectByIndex(static_cast<int>(clientID))->SetActive(false);

	m_Network->DisableUserMapEntry(clientID);

	//Send everyone the disconnected user
	ClientDisconnectPacket clientDisconnectPacket;
	clientDisconnectPacket.ClientID = clientID;
	m_Network->Send(&clientDisconnectPacket, HIGH_PRIORITY, RELIABLE_ORDERED, true, m_Packet->systemAddress);

	//Send everyone current connected clients
	CurrentClientsPacket currentClientsPacket;
	currentClientsPacket.CurrentClients = m_Network->GetCurrentClients();
	m_Network->Send(&currentClientsPacket, HIGH_PRIORITY, RELIABLE_ORDERED, true, m_Packet->systemAddress);

	//Assign lobby lead to first connected user in line
	if (m_Game->GetLobbyLead() == m_Packet->systemAddress)
	{
		bool newLeadAssigned = false;
		for (unsigned int i = 0; i < m_Network->GetUserMap().size(); i++)
		{
			User& user = m_Network->GetClientByID(i);

			if (user.Connected == true)
			{
				LobbyLeadPacket lobbyLead;
				lobbyLead.Lead = true;
				m_Network->Send(&lobbyLead, HIGH_PRIORITY, RELIABLE_ORDERED, false, user.SystemAddress);
				m_Game->SetLoddyLead(user.SystemAddress);
				newLeadAssigned = true;
				break;
			}
		}

		if (!newLeadAssigned)
			m_Game->SetLoddyLead(SLNet::UNASSIGNED_SYSTEM_ADDRESS);
	}
}

void PacketParser::ParseNewConnection()
{
	unsigned int clientID = m_Network->GetClientID();
	//assert((clientID >= MAXPLAYERS) && "Client ID surpassed max players");

	NewClientPacket newClient;
	newClient.ClientID = clientID;
	newClient.MaxPlayers = m_Game->GetMaxPlayers();
	newClient.MaxCrates = m_Game->GetMaxCrates();
	newClient.MaxBombs = m_Game->GetMaxBombs();
	newClient.MaxUpgrades = m_Game->GetMaxUpgrades();
	
	LobbyLeadPacket lobbyLead;

	if (m_Game->GetLobbyLead() == SLNet::UNASSIGNED_SYSTEM_ADDRESS)
	{
		m_Game->SetLoddyLead(m_Packet->systemAddress);
		lobbyLead.Lead = true;
	}

	m_Network->Send(&newClient, HIGH_PRIORITY, RELIABLE_ORDERED, false, m_Packet->systemAddress);
	m_Network->Send(&lobbyLead, HIGH_PRIORITY, RELIABLE_ORDERED, false, m_Packet->systemAddress);
}

void PacketParser::ParseStartGame()
{
	StartGamePacket* packet = (StartGamePacket*)m_Packet->data;

	if (m_Game->GetLobbyLead() == m_Packet->systemAddress)
	{
		if (m_Network->GetUserMap().size() >= 2 && m_Network->GetUserMap().size() <= MAXPLAYERS)
		{
			m_Game->SetStartGame(packet->StartGame);

			//Send the position of all players to each client
			for (unsigned int i = 0; i < m_Network->GetUserMap().size(); i++) //Shouldnt be map size
			{
				if (m_Network->GetClientByID(i).Connected == true)
				{
					MovementPacket movementPacket;
					movementPacket.ClientID = i;
					movementPacket.Position = m_Engine->GetObjectByIndex(i)->GetTransform().position;
					movementPacket.CellID = m_Engine->GetObjectByIndex(i)->GetCell();
					m_Network->Send(&movementPacket);

					m_Engine->GetObjectByIndex(i)->SetActive(true);
					packet->ClientsConnected[i] = true;
				}
				else
					packet->ClientsConnected[i] = false;
			}

			m_Network->Send(packet);
		}
	}
}

void PacketParser::ParseName()
{
	NamePacket* packet = (NamePacket*)m_Packet->data;

	m_Network->AddUserMapEntry(User{ packet->Name, m_Packet->systemAddress, true });

	CurrentClientsPacket currentClients;
	currentClients.CurrentClients = m_Network->GetCurrentClients();
	m_Network->Send(&currentClients);
}

void PacketParser::ParseChat()
{
	ChatPacket* packet = (ChatPacket*)m_Packet->data;
	m_Network->Send(packet, HIGH_PRIORITY, RELIABLE_ORDERED, true, m_Packet->systemAddress);
}

void PacketParser::ParseMovement()
{
	MovementPacket* packet = (MovementPacket*)m_Packet->data;

	//Validate if the position is possible
	//Note: Currently we check if the cell it points to is valid and not out of bounds.
	//However, ideally we would check if the cell is valid, not out of bounds & if its next to the original cell
	//This solution however might be too expensive on a large scale, therefor we currently don't do this.

	//Out of bounds check | Maybe not needed anymore
	//float offset = 5.0f; //Avoid getting stuck in walls
	//if (packet->Position.x >= (((MAPSIZEX - 2) * TILESIZE) + offset) || packet->Position.x <= (TILESIZE - offset)
	//	|| packet->Position.y >= (((MAPSIZEY - 2) * TILESIZE) + offset) || packet->Position.y <= (TILESIZE - offset))
	//	return;

	//Note: if the offset is too small, look into repositioning players so they stick to the center of a cell.
	//When this is in place, we can increase the offset and just correct the player if needed.

	int cell = (static_cast<int>(packet->Position.x + (TILESIZE * 0.5f)) / static_cast<int>(TILESIZE) +
		(static_cast<int>(packet->Position.y + (TILESIZE * 0.5f)) / static_cast<int>(TILESIZE)) * MAPSIZEX);

	if (m_Engine->m_Map[cell] == EMPTYCELL)
		if (cell == packet->CellID)
		{
			m_Engine->GetObjectByIndex(packet->ClientID)->GetTransform().position = packet->Position;
			m_Engine->GetObjectByIndex(packet->ClientID)->SetCell(packet->CellID);

			m_Network->Send(packet, LOW_PRIORITY, UNRELIABLE_SEQUENCED, true, m_Packet->systemAddress);
		}
}

void PacketParser::ParseTest()
{
	TestPacket* packet = (TestPacket*)m_Packet->data;

	printf("Test has been parsed\n");
	printf("X: %i, Y: %i, Z: %i\n", packet->X, packet->Y, packet->Z);
}