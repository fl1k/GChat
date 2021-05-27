#include <iostream>
#include <GNet/Core/Network.h>
#include "Misc/Logger.h"
#include "Server/Server.h"

int main()
{
	if (GNet::Network::Initialize() == false)
	{
		Logger::Write("Failed initializing network.", Logger::Reason::Error);
		GNet::Network::Shutdown();
		system("pause");
		return 0;
	}
	Logger::Write("Successfully initialized network.");
	Server server(9920, 500);
	if (server.Initialize() == false)
	{
		Logger::Write("Failed initializing server.", Logger::Reason::Error);
		GNet::Network::Shutdown();
		system("pause");
		return 0;
	}

	while (server.Present());
	GNet::Network::Shutdown();
}