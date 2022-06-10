#include "Precomp.hpp"
#include "Game.hpp"

int main()
{
	//Set the display of the Raspbarry Pi correct
#ifdef LINUX
	setenv("DISPLAY", ":0", 1);
#endif

	std::unique_ptr<Game> game = std::make_unique<Game>();

	game->Initialize();

	game->UpdateLoop();
}