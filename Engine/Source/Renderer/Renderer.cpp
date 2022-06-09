#include "Precomp.hpp"
#include "Renderer/Renderer.hpp"
#include "ObjectManager/Object.hpp"

#include "Renderer/Mesh/BaseMesh.hpp"
#include "Renderer/Mesh/CircleMesh.hpp"
#include "Renderer/Mesh/RectMesh.hpp"

Renderer::Renderer()
{
	m_Window.create(sf::VideoMode(SCREENWIDTH, SCREENHEIGHT), "Network-Project");
	m_Window.setFramerateLimit(FRAMERATE);
	ImGui::SFML::Init(m_Window);
}

Renderer::~Renderer()
{
	ImGui::SFML::Shutdown();
}

void Renderer::Render(const std::vector<std::shared_ptr<Object>>& objects)
{
	for (auto& object : objects)
	{
		if (object->GetActive())
		{
			switch (object->GetMeshType())
			{
			case MeshType::Circle:
			{
				CircleMesh* circleMesh = reinterpret_cast<CircleMesh*>(object->GetMesh());
				DrawCircle(circleMesh->m_Diameter, CastVec2(object->GetTransform().position), CastVec4(circleMesh->m_Color));
			}
			break;
			case MeshType::Rect:
			{
				RectMesh* rectMesh = reinterpret_cast<RectMesh*>(object->GetMesh());
				DrawRectangle(CastVec2(rectMesh->m_ShapeSize), CastVec2(object->GetTransform().position), CastVec4(rectMesh->m_Color));
			}
			break;
			}
		}
	}

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

//Util
const sf::Vector2f Renderer::CastVec2(const glm::vec2& value) const
{
	return sf::Vector2f(value.x, value.y);
}

const sf::Color Renderer::CastVec4(const glm::vec4& value) const
{
	return sf::Color(value.x, value.y, value.z);
}