#include "Precomp.h"
#include "Renderer.h"
#include "Client.h"
#include "Player.h"
#include "Game.h"

#include <memory>

int main()
{
	//Initialize variables
	std::shared_ptr<Game> game = std::make_shared<Game>();
	std::unique_ptr<Client> client = std::make_unique<Client>(game);
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


		if (renderer->GetWindow().hasFocus())
		{
			if (!game->GetPlayers().empty())
				//Move my sphere
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
				{
					game->GetPlayers()[client->m_ClientNumber]->SetPosition(game->GetPlayers()[client->m_ClientNumber]->GetPosition().x + 5, game->GetPlayers()[client->m_ClientNumber]->GetPosition().y); //Miss deltatime here
					client->SendPos();
				}
		}

		if(!game->GetPlayers().empty())
			for (unsigned int i = 0; i < game->GetPlayers().size(); i++)
			{
				renderer->DrawCircle(game->GetPlayers()[i]->GetDiameter(), game->GetPlayers()[i]->GetPosition(), game->GetPlayers()[i]->GetColor());
			}

		//Draw something
		//renderer->DrawCircle(50.0f, sf::Vector2f(10, 10), sf::Color(sf::Color::Cyan));
		//renderer->DrawRectangle(sf::Vector2f(150, 100), sf::Vector2f(140, 140), sf::Color(sf::Color::Green));

		//Game
		game->Update();

		//Render all
		renderer->Render();
	}
}