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
	: m_Name("")
	, m_Port("")
	, m_IsDetailSet(false)
	, m_MaxPlayerObjects(0)
	, m_MaxCrateObjects(0)
	, m_MaxBombObjects(0)
	, m_MaxUpgradeObjects(0)
	, m_MainMenu(true)
	, m_StartGame(false)
	, m_CloseGame(false)
	, m_LobbyLead(false)
	, m_ObjectsInit(false)
{}

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

	m_ObjectsInit = true;
}

//Gameloop
void Game::UpdateLoop()
{
	while (m_Engine->IsWindowOpen() && !m_CloseGame)
	{
		//Call at the start of the loop
		m_Engine->Begin();

		//Main menu
		if (m_MainMenu)
		{
			//Show start & exit button
			ImGui::Begin("Main_Menu");
			if (ImGui::Button("Start"))
				m_MainMenu = false;
			if (ImGui::Button("Quit"))
				m_CloseGame = true;
			ImGui::End();
		}
		else
		{
			//Fill in client details
			if (!m_IsDetailSet)
			{
				//Set name & port
				SetClientDetails();
				if (m_Engine->IsKeyPressed(sf::Keyboard::Enter))
				{
					m_Network->SetServerPort(atoi(m_Port.c_str()));
					m_Network->SetServerIP(m_ServerIP);
					m_IsDetailSet = true;
				}

				//Quick start for debugging
				ImGui::Begin("Quick Launch");
					if (ImGui::Button("Stefan"))
					{
						m_Name = "Stefan";
						m_Port = "40000";
						m_Network->SetServerPort(atoi(m_Port.c_str()));
						m_Network->SetServerIP("127.0.0.1");
						m_IsDetailSet = true;
					}
					if (ImGui::Button("Wessel"))
					{
						m_Name = "Wessel";
						m_Port = "40000";
						m_Network->SetServerPort(atoi(m_Port.c_str()));
						m_Network->SetServerIP("127.0.0.1");
						m_IsDetailSet = true;
					}
					if (ImGui::Button("Lex"))
					{
						m_Name = "Lex";
						m_Port = "40000";
						m_Network->SetServerPort(atoi(m_Port.c_str()));
						m_Network->SetServerIP("127.0.0.1");
						m_IsDetailSet = true;
					}
					if (ImGui::Button("Stan"))
					{
						m_Name = "Stan";
						m_Port = "40000";
						m_Network->SetServerPort(atoi(m_Port.c_str()));
						m_Network->SetServerIP("127.0.0.1");
						m_IsDetailSet = true;
					}
					if (ImGui::Button("?????"))
					{
						m_Name = "?????";
						m_Port = "40000";
						m_Network->SetServerPort(atoi(m_Port.c_str()));
						m_Network->SetServerIP("127.0.0.1");
						m_IsDetailSet = true;
					}
				ImGui::End();
			}

			//Attempt connection
			if (!m_Network->GetConnectingAttempt() && m_IsDetailSet)
			{
				m_Network->SetConnection();
			}
			else if (m_Network->GetIsConnected()) //If connected
			{
				//Lobby chat before you start the game
				if (!m_StartGame)
				{
					//Chat window
					ImGui::Begin("Chat Box");
					ChatInput();
					SendChat();
					DisplayChat();

					//Start game if lobby lead
					if (m_LobbyLead)
					{
						if (ImGui::Button("Start Game"))
						{
							if (m_Network->GetCurrentClients() >= 2 && m_Network->GetCurrentClients() <= MAXPLAYERS)
							{

								StartGamePacket startGamePacket;
								startGamePacket.StartGame = true;
								m_Network->Send(&startGamePacket);
							}
						}	
					}
					ImGui::End();
				}
				else //Start the gameplay
				{
					if (m_LobbyLead)
					{
						ImGui::Begin("Menu");
						if (ImGui::Button("Back to menu"))
						{
							StartGamePacket startGamePacket;
							startGamePacket.StartGame = false;
							m_Network->Send(&startGamePacket);
						}
						ImGui::End();
					}
					

					if (m_Engine->HasFocus())
					{
						//Input
						if (m_Engine->IsKeyPressed(sf::Keyboard::W))
						{
							m_Engine->GetObjectByIndex(m_Network->GetClientID())->GetTransform().position.y = m_Engine->GetObjectByIndex(m_Network->GetClientID())->GetTransform().position.y + (-260.f * m_Engine->GetDeltaTime());
						}
						if (m_Engine->IsKeyPressed(sf::Keyboard::S))
						{
							m_Engine->GetObjectByIndex(m_Network->GetClientID())->GetTransform().position.y = m_Engine->GetObjectByIndex(m_Network->GetClientID())->GetTransform().position.y + (260.f * m_Engine->GetDeltaTime());
						}
						if (m_Engine->IsKeyPressed(sf::Keyboard::A))
						{
							m_Engine->GetObjectByIndex(m_Network->GetClientID())->GetTransform().position.x = m_Engine->GetObjectByIndex(m_Network->GetClientID())->GetTransform().position.x + (-260.f * m_Engine->GetDeltaTime());
						}
						if (m_Engine->IsKeyPressed(sf::Keyboard::D))
						{
							m_Engine->GetObjectByIndex(m_Network->GetClientID())->GetTransform().position.x = m_Engine->GetObjectByIndex(m_Network->GetClientID())->GetTransform().position.x + (260.f * m_Engine->GetDeltaTime());
						}
					}

					//Send current position to the server
					MovementPacket movementPacket;
					movementPacket.ClientID = m_Network->GetClientID();
					movementPacket.Position = m_Engine->GetObjectByIndex(m_Network->GetClientID())->GetTransform().position;
					m_Network->Send(&movementPacket, LOW_PRIORITY, UNRELIABLE_SEQUENCED);
				}
			}
			else //If not connected
			{
				printf("Not yet connected...\n");
			}
		}

		//Call at the end of the loop
		m_Engine->End();
	}
}

//Set name & port
void Game::SetClientDetails()
{
	char tempName[512];
	char tempPort[512];
	char tempIP[512];

#ifdef WINDOWS
	strcpy_s(tempName, m_Name.c_str());
	strcpy_s(tempPort, m_Port.c_str());
	strcpy_s(tempIP, m_ServerIP.c_str());
#else
	strcpy(tempName, m_Name.c_str());
	strcpy(tempPort, m_Port.c_str());
	strcpy(tempIP, m_ServerIP.c_str());
#endif

	ImGui::Begin("Setup");
	ImGui::InputText("Set Name:", tempName, sizeof(tempName));
	ImGui::InputText("Set Port:", tempPort, sizeof(tempPort));
	ImGui::InputText("Set IP:", tempIP, sizeof(tempIP));
	ImGui::End();

	m_Name = tempName;
	m_Port = tempPort;
	m_ServerIP = tempIP;
}

//Obtain message to send
void Game::ChatInput()
{
	char message[MAXCHATLENGTH] = "";
	ImGui::InputText("", message, sizeof(message));

	if (m_Engine->HasFocus())
	{
		if (m_Engine->IsKeyPressed(sf::Keyboard::Enter))
		{
#ifdef WINDOWS
			strcpy_s(m_Message, message);
#else
			strcpy(m_Message, message);
#endif
		}
	}
}

//Send chat message
void Game::SendChat()
{
	//Check if the message has data
	if (m_Message[0] != '\0')
	{
		ChatPacket chatPacket;

		//Add name prefix
		std::string prefix = m_Name + ": ";
#ifdef WINDOWS
		strncpy_s(chatPacket.Message, prefix.c_str(), sizeof(chatPacket.Message));
		strncat_s(chatPacket.Message, m_Message, sizeof(chatPacket.Message) - strlen(prefix.c_str()) - 1);
#else
		strncpy(chatPacket.Message, prefix.c_str(), sizeof(chatPacket.Message));
		strncat(chatPacket.Message, m_Message, sizeof(chatPacket.Message) - strlen(prefix.c_str()) - 1);
#endif

		//Add message to display vector
		AddChatToArray(chatPacket.Message);

		//Send message
		m_Network->Send(&chatPacket);

		m_Message[0] = '\0';
	}
}

//Show chat messages
void Game::DisplayChat()
{
	for (unsigned int i = 0; i < m_MessageVector.size(); i++)
	{
		ImGui::Text(m_MessageVector[i].c_str());
	}
}

//Add chat message to message vector
void Game::AddChatToArray(const char* message)
{
	if (m_MessageVector.size() < MAXCHATMESSAGES)
		m_MessageVector.push_back(message);
	else if (m_MessageVector.size() >= MAXCHATMESSAGES)
	{
		m_MessageVector.erase(m_MessageVector.begin());
		m_MessageVector.push_back(message);
	}
}

//Getters
const std::string& Game::GetName() const
{
	return m_Name;
}

const bool Game::GetObjectInit() const
{
	return m_ObjectsInit;
}

//Setters
void Game::SetMaxObjects(unsigned int maxPlayers, unsigned int maxCrates, unsigned int maxBombs, unsigned int maxUpgrades)
{
	m_MaxPlayerObjects = maxPlayers;
	m_MaxCrateObjects = maxCrates;
	m_MaxBombObjects = maxBombs;
	m_MaxUpgradeObjects = maxUpgrades;
}

void Game::SetStartGame(bool startGame)
{
	m_StartGame = startGame;
}

void Game::SetLobbyLead(bool lobbyLead)
{
	m_LobbyLead = lobbyLead;
}