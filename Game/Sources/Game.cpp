#include "Precomp.h"
#include "Game.h"
#include "Player.h"

Game::Game() :
	m_DeltaTime(0),
	m_StartTime(std::chrono::system_clock::now()),
	m_Frames(0),
	m_ElapsedTime(0.0f),
	m_Players(NULL)
{}

Game::~Game()
{}

//Game loop
void Game::Update()
{
	//Print fps in console
	m_ElapsedTime += m_DeltaTime.count();
	m_Frames++;
	if (m_ElapsedTime > 1.0f)
	{
		printf("deltatime %f\n", m_DeltaTime.count());
		printf("Average frame time: %f | FPS: %i\n", m_ElapsedTime / static_cast<float>(m_Frames), m_Frames);
		m_ElapsedTime = 0.0f;
		m_Frames = 0;
	}

	//for (int i = 0; i < m_Players.size(); i++)
	//{
		//Get the received position of the clients
		//And change their position to the new one
	//}


	//Deltatime
	m_DeltaTime = std::chrono::system_clock::now() - m_StartTime;
	m_StartTime = std::chrono::system_clock::now();
}

//Remove a player from the vector
void Game::ErasePlayer(unsigned int position)
{
	m_Players.erase(m_Players.begin() + position);
}

//Getters
const std::vector<std::unique_ptr<Player>>& Game::GetPlayers() const
{
	return m_Players;
}

//Setters
void Game::SetPlayer(float diameter, sf::Vector2f position, sf::Color color)
{
	m_Players.push_back(std::make_unique<Player>(position, color, diameter));
}