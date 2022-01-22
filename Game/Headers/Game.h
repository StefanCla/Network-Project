#pragma once
#include <chrono>
#include <vector>
#include <SFML/Graphics.hpp> //SFML variables

class Player;

class Game
{
public:
	Game();
	~Game();

	void Update();
	void ErasePlayer(unsigned int position);

	//Getters
	const std::vector<std::unique_ptr<Player>>& GetPlayers() const;

	//Setters
	void SetPlayer(float diameter, sf::Vector2f position, sf::Color color);

private:
	//Deltatime
	std::chrono::system_clock::time_point m_StartTime;
	std::chrono::duration<float> m_DeltaTime;
	float m_ElapsedTime;
	unsigned int m_Frames;

	//Gameplay variables
	std::vector<std::unique_ptr<Player>> m_Players;
};