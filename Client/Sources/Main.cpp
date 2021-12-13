#include "Precomp.h"
#include "Renderer.h"
#include "Client.h"

#include <memory>

int main()
{
	//Initialize variables
	std::unique_ptr<Client> client = std::make_unique<Client>();
	std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>();

	bool IsConnected = false;

	//Update loop
	while (renderer->GetWindow().isOpen())
	{
		//Input
		while (renderer->GetWindow().pollEvent(renderer->GetEvent()))
		{
			ImGui::SFML::ProcessEvent(renderer->GetEvent());
			if (renderer->GetEvent().type == sf::Event::Closed)
				renderer->GetWindow().close();
		}

		//Update ImGui
		renderer->UpdateUI();

		//Create connection & update the receiver
		if (client->m_Name.empty())
		{
			client->SetName();
		}
		else
		{
			if (!IsConnected)
			{
				client->SetConnection();
				IsConnected = true;
			}
			else
			{
				ImGui::Begin("Chat box");

				client->GetInput();
				client->Receive();
				client->DisplayChat();

				ImGui::End();
			}
		}

		//Draw something
		//renderer->DrawCircle(50.0f, sf::Vector2f(10, 10), sf::Color(sf::Color::Cyan));
		//renderer->DrawRectangle(sf::Vector2f(150, 100), sf::Vector2f(140, 140), sf::Color(sf::Color::Green));

		//Render all
		renderer->Render();
	}
}