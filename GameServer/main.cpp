/**
* @file main.cpp
*
* @author Hourui (liquidmonkey)
**/

#include "GameServer.hpp"
#include "Utility/logger.hpp"
using namespace UProject;

int main(int argc, char* argv[])
{
	logger kLogger(logger::log_level::debug);
	Clog::active_logger(&kLogger);

	GameServer* server = GameServer::GetInstance();
	if (server->Start(argc, argv))
		server->Run();

#ifdef _WIN32
	system("pause");
#endif
	return 0;
}