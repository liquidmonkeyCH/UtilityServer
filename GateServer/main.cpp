/**
* @file main.cpp
*
* @author Hourui (liquidmonkey)
**/

#include "GateServer.hpp"
#include "Utility/logger.hpp"
using namespace UProject;

int main(int argc, char* argv[])
{
	GateServer* server = GateServer::GetInstance();
	if (server->Start(argc, argv))
		server->Run();

#ifdef _WIN32
	system("pause");
#endif
	return 0;
}