#include "Precomp.h"
#include "Player.h"

Player::Player() :
	m_Position(sf::Vector2f(0.0f, 0.0f)),
	m_Color(sf::Color(255, 255, 255, 255)),
	m_Diameter(10.0f)
{}

Player::Player(sf::Vector2f position, sf::Color color, float diameter) :
	m_Position(position),
	m_Color(color),
	m_Diameter(diameter)
{}

Player::~Player()
{}

//void Player::Update(float deltaTime)
//{
//
//}

//Setters
void Player::SetPosition(sf::Vector2f position)
{
	m_Position = position;
}

void Player::SetPosition(float posX, float posY)
{
	m_Position = sf::Vector2f(posX, posY);
}

void Player::SetColor(sf::Color color)
{
	m_Color = color;
}

void Player::SetDiameter(float diameter)
{
	m_Diameter = diameter;
}

//Getters
sf::Vector2f Player::GetPosition() const
{
	return m_Position;
}

sf::Color Player::GetColor() const
{
	return m_Color;
}

float Player::GetDiameter() const
{
	return m_Diameter;
}