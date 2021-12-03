#include "Precomp.h"
#include "Server.h"

int main()
{
	Server* server = new Server();
	server->PrintNumber(TEST);

	delete server;
}