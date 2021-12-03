#include "Precomp.h"
#include "Renderer.h"

//Constructor
Renderer::Renderer()
{
	m_Window.create(sf::VideoMode(1280, 800), "Network-Project");
	m_Window.setFramerateLimit(240);
	ImGui::SFML::Init(m_Window);
}

Renderer::~Renderer()
{
	ImGui::SFML::Shutdown();
}	

//Functions
void Renderer::Render()
{
	ImGui::SFML::Render(m_Window);
	m_Window.display();
}

void Renderer::UpdateUI()
{
	m_Window.clear(sf::Color(0, 0, 0, 0));
	ImGui::SFML::Update(m_Window, m_SFMLDeltaTime.restart());
}

//Drawing
void Renderer::DrawCircle(float diameter, sf::Vector2f position, sf::Color color)
{
	sf::CircleShape circle(diameter);
	circle.setPosition(position);
	circle.setFillColor(color);

	m_Window.draw(circle);
}

void Renderer::DrawRectangle(sf::Vector2f shapeSize, sf::Vector2f position, sf::Color color)
{
	sf::RectangleShape rectangle(shapeSize);
	rectangle.setPosition(position);
	rectangle.setFillColor(color);

	m_Window.draw(rectangle);
}

//Getters
sf::RenderWindow& Renderer::GetWindow()
{
	return m_Window;
}

sf::Event& Renderer::GetEvent()
{
	return m_Event;
}