#include "Precomp.hpp"
#include "Engine.hpp"
#include <iostream>
#include "Network/Network.hpp"

Engine::Engine()
	: m_ObjectManager(std::make_unique<ObjectManager>())
{
#ifndef SERVER
	m_Renderer = std::make_unique<Renderer>();
#endif
}

Engine::~Engine()
{}

void Engine::Initialize()
{

}

void Engine::Begin()
{
	//Deltatime start
	m_ElapsedTime += m_DeltaTime.count();
	m_Frames++;
	if (m_ElapsedTime >= 1.0f) //1 second
	{
		printf("Deltatime %f\n", m_DeltaTime.count());
		printf("Average frame time: %f | FPS: %i\n", m_ElapsedTime / static_cast<float>(m_Frames), m_Frames);
		m_ElapsedTime = 0.0f;
		m_Frames = 0;
	}

	//Get network data
	if(m_Network->m_Self != nullptr)
		if(m_Network->GetConnectingAttempt())
			m_Network->Receive();

#ifndef SERVER
	while (m_Renderer->GetWindow().pollEvent(m_Renderer->GetEvent()))
	{
		ImGui::SFML::ProcessEvent(m_Renderer->GetEvent());
		if (m_Renderer->GetEvent().type == sf::Event::Closed)
			m_Renderer->GetWindow().close();
	}

	m_Renderer->UpdateUI();
#endif
}

void Engine::End()
{
	//ObjectManager
	m_ObjectManager->Update(m_DeltaTime.count());

	//Render
#ifndef SERVER
	m_Renderer->Render(m_ObjectManager->GetObjects());
#endif

	//Deltatime end
	m_DeltaTime = std::chrono::system_clock::now() - m_StartTime;
	m_StartTime = std::chrono::system_clock::now();
}

void Engine::SetObjects(std::vector<std::shared_ptr<Object>>& objects)
{
	m_ObjectManager->SetObjects(objects);
}

bool Engine::IsWindowOpen()
{
#ifdef SERVER
	return true;
#else
	return m_Renderer->GetWindow().isOpen();
#endif
}

bool Engine::HasFocus()
{
#ifdef SERVER
	return true;
#else
	return m_Renderer->GetWindow().hasFocus();
#endif
}

#ifdef CLIENT
bool Engine::IsKeyPressed(sf::Keyboard::Key key)
{
	return sf::Keyboard::isKeyPressed(key);
}
#endif

float Engine::GetDeltaTime()
{
	return m_DeltaTime.count();
}

void Engine::SetNetworkComponent(std::shared_ptr<Network> network)
{
	m_Network = network;
}

std::shared_ptr<Object> Engine::GetObjectByIndex(int index)
{
	return m_ObjectManager->GetObjectByIndex(index);
}