#include "Precomp.h"
#include "Server.h"
#include "Game.h"

#include <memory>

int main()
{
	//Initialize variables
	std::shared_ptr<Game> game = std::make_unique<Game>();
	std::unique_ptr<Server> server = std::make_unique<Server>(game);

	server->SetConnection();

	while (!server->m_Shutdown)
	{
		game->Update();
		server->Receive();
	}
}