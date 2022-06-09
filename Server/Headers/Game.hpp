#pragma once
#include <memory>
#include <string>

#include "Objects/TestObject.hpp"
#include "slikenet/peerinterface.h"

class PacketParser;
class Network;
class Engine;
class Object;

class Game
{
public:
	Game();
	~Game();

	void Initialize();
	void UpdateLoop();

	//Setters
	void SetMaxObjects(unsigned int maxPlayers, unsigned int maxCrates, unsigned int maxBombs, unsigned int maxUpgrades);
	void SetLoddyLead(SLNet::SystemAddress systemAddress);
	void SetStartGame(bool startGame);

	//Getters
	const unsigned int GetMaxPlayers() const;
	const unsigned int GetMaxCrates() const;
	const unsigned int GetMaxBombs() const;
	const unsigned int GetMaxUpgrades() const;

	const SLNet::SystemAddress GetLobbyLead() const;

	//Core
private:
	std::shared_ptr<Network> m_Network;
	std::unique_ptr<Engine> m_Engine;

	//Variables
private:
	unsigned int m_MaxPlayerObjects;
	unsigned int m_MaxCrateObjects;
	unsigned int m_MaxBombObjects;
	unsigned int m_MaxUpgradeObjects;

	SLNet::SystemAddress m_LobbyLead;
	bool m_StartGame;

	glm::vec2 m_PlayerStartLocation[MAXPLAYERS];

	//Functions
private:
	void InititalizeGameObjects();

};