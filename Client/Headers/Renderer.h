#pragma once
#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

class Renderer
{
public:
	Renderer();
	~Renderer();

	void Render();
	void UpdateUI();

	void DrawCircle(float diameter, sf::Vector2f position, sf::Color color);
	void DrawRectangle(sf::Vector2f shapeSize, sf::Vector2f position, sf::Color color);

	//Getters
	sf::RenderWindow& GetWindow();
	sf::Event& GetEvent();

private:
	sf::RenderWindow m_Window;
	sf::Clock m_SFMLDeltaTime;
	sf::Event m_Event;
};