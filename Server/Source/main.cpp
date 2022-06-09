#include "Precomp.hpp"
#include "Game.hpp"

int main()
{
	std::unique_ptr<Game> game = std::make_unique<Game>();

	game->SetMaxObjects(MAXPLAYERS, MAXCRATES, MAXBOMBS, MAXUPGRADES);
	game->Initialize();

	game->UpdateLoop();
}