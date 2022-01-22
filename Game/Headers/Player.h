#pragma once
#include <SFML/Graphics.hpp>

class Player
{
public:
	Player();
	Player(sf::Vector2f position, sf::Color color, float diameter);
	~Player();

	/*void Update(float deltaTime);*/

	//Setters
	void SetPosition(sf::Vector2f position);
	void SetPosition(float posX, float posY);
	void SetColor(sf::Color color);
	void SetDiameter(float diameter);

	//Getters
	sf::Vector2f GetPosition() const;
	sf::Color GetColor() const;
	float GetDiameter() const;

private:
	sf::Vector2f m_Position;
	sf::Color m_Color;
	float m_Diameter;

};