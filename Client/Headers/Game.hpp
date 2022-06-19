#pragma once
#include <memory>
#include <string>
#include <vector>

#include "Objects/TestObject.hpp"

class PacketParser;
class Network;
class Engine;
class Object;

enum class Direction
{
	Up,
	Down,
	Left,
	Right
};

class Game
{
public:
	Game();
	~Game();

	void Initialize();
	void InititalizeGameObjects();
	void UpdateLoop();

	//Getters
	const std::string& GetName() const;
	const bool GetObjectInit() const;

	//Setters
	void SetMaxObjects(unsigned int maxPlayers, unsigned int maxCrates, unsigned int maxBombs, unsigned int maxUpgrades);
	void AddChatToArray(const char* message);
	void SetStartGame(bool startGame);
	void SetLobbyLead(bool lobbyLead);

	//Core
private:
	std::shared_ptr<Network> m_Network;
	std::unique_ptr<Engine> m_Engine;

	//Variables
private:
	std::string m_Name;
	std::string m_Port;
	std::string m_ServerIP;
	bool m_IsDetailSet;

	std::vector<std::string> m_MessageVector;
	char m_Message[MAXCHATLENGTH];

	unsigned int m_MaxPlayerObjects;
	unsigned int m_MaxCrateObjects;
	unsigned int m_MaxBombObjects;
	unsigned int m_MaxUpgradeObjects;

	bool m_MainMenu;
	bool m_StartGame;
	bool m_CloseGame;
	bool m_LobbyLead;
	bool m_ObjectsInit;

	//Functions
private:
	void SetClientDetails();
	void MovePlayer(Direction direction);
	void SetPosAndCell(std::shared_ptr<Object> player, float speed, float& pos);

	//Chat
	void ChatInput();
	void SendChat();
	void DisplayChat();

};