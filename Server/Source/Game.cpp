#include "Precomp.hpp"
#include "Game.hpp"
#include "Engine.hpp"

//Networking
#include "Network/CustomPackets.hpp"
#include "Network/PacketParser.hpp"
#include "Network/Network.hpp"

#include "ObjectManager/Object.hpp"
#include "Objects/TestObject.hpp"
#include "Objects/PlayerObject.hpp"
#include "Objects/CrateObject.hpp"
#include "Objects/BombObject.hpp"
#include "Objects/UpgradeObject.hpp"

Game::Game()
	: m_MaxPlayerObjects(0)
	, m_MaxCrateObjects(0)
	, m_MaxBombObjects(0)
	, m_MaxUpgradeObjects(0)
	, m_StartGame(false)
	, m_LobbyLead(SLNet::UNASSIGNED_SYSTEM_ADDRESS)
{
	m_PlayerStartLocation[0] = glm::vec2(32.0f, 32.0f);
	m_PlayerStartLocation[1] = glm::vec2(224.0f, 224.0f);
	m_PlayerStartLocation[2] = glm::vec2(32.0f, 224.0f);
	m_PlayerStartLocation[3] = glm::vec2(224.0f, 32.0f);

	m_StartCell[0] = 10;
	m_StartCell[1] = 70;
	m_StartCell[2] = 16;
	m_StartCell[3] = 63;
}

Game::~Game()
{}

void Game::Initialize()
{
	//Create
	m_Engine = std::make_unique<Engine>();
	m_Network = std::make_shared<Network>(std::make_shared<PacketParser>(m_Engine.get(), this));

	//Initialize / setup
	m_Engine->Initialize();
	m_Engine->SetNetworkComponent(m_Network);

	InititalizeGameObjects();

	m_Network->SetConnection();
}

void Game::InititalizeGameObjects()
{
	std::vector<std::shared_ptr<Object>> objects;

	//Add all objects to the vector
	for (unsigned int i = 0; i < m_MaxPlayerObjects; i++)
	{
		objects.emplace_back(std::make_shared<PlayerObject>(i));
	}

	for (unsigned int i = 0; i < m_MaxCrateObjects; i++)
	{
		objects.emplace_back(std::make_shared<CrateObject>());
	}

	for (unsigned int i = 0; i < m_MaxBombObjects; i++)
	{
		objects.emplace_back(std::make_shared<BombObject>());
	}

	for (unsigned int i = 0; i < m_MaxUpgradeObjects; i++)
	{
		objects.emplace_back(std::make_shared<UpgradeObject>());
	}

	m_Engine->SetObjects(objects);

	//Set the positions of the players
	for (unsigned int i = 0; i < MAXPLAYERS; i++)
	{
		m_Engine->GetObjectByIndex(i)->GetTransform().position = m_PlayerStartLocation[i];
	}
}

//Gameloop
void Game::UpdateLoop()
{
	while (m_Engine->IsWindowOpen())
	{
		//Call at the start of the loop
		m_Engine->Begin();

		if (m_StartGame)
		{
			printf("The game loop is running!\n");
			//Gameloop here, only lobby leader can start
		}

		//Call at the end of the loop
		m_Engine->End();
	}
}

//Setters
void Game::SetMaxObjects(unsigned int maxPlayers, unsigned int maxCrates, unsigned int maxBombs, unsigned int maxUpgrades)
{
	m_MaxPlayerObjects = maxPlayers;
	m_MaxCrateObjects = maxCrates;
	m_MaxBombObjects = maxBombs;
	m_MaxUpgradeObjects = maxUpgrades;
}

void Game::SetLoddyLead(SLNet::SystemAddress systemAddress)
{
	m_LobbyLead = systemAddress;
}

void Game::SetStartGame(bool startGame)
{
	m_StartGame = startGame;

	//Reset player values
	for (unsigned int i = 0; i < MAXPLAYERS; i++)
	{
		m_Engine->GetObjectByIndex(i)->GetTransform().position = m_PlayerStartLocation[i];
		m_Engine->GetObjectByIndex(i)->SetCell(m_StartCell[i]);
	}
}

//Getters
const unsigned int Game::GetMaxPlayers() const
{
	return m_MaxPlayerObjects;
}

const unsigned int Game::GetMaxCrates() const
{
	return m_MaxCrateObjects;
}

const unsigned int Game::GetMaxBombs() const
{
	return m_MaxBombObjects;
}

const unsigned int Game::GetMaxUpgrades() const
{
	return m_MaxUpgradeObjects;
}

const SLNet::SystemAddress Game::GetLobbyLead() const
{
	return m_LobbyLead;
}