#pragma once
#include <memory>
#include <ImGui/Headers/imgui.h>
#include <ImGui/Headers/imgui-SFML.h>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

class Object;

class Renderer
{
public:
	Renderer();
	~Renderer();

	void Render(const std::vector<std::shared_ptr<Object>>& objects);
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

private:
	const sf::Vector2f CastVec2(const glm::vec2& value) const;
	const sf::Color CastVec4(const glm::vec4& value) const;
};