#include "Precomp.h"
#include "Server.h"

#include <memory>

int main()
{
	//Initialize variables
	std::unique_ptr<Server> server = std::make_unique<Server>();
	server->SetConnection();

	while (!server->m_Shutdown)
	{
		server->Receive();
	}
}