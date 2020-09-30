/**
* @file main.cpp
*
* @author Hourui (liquidmonkey)
**/

#include "GateServer.hpp"
#include <Chrono>
using namespace UProject;

int main(int argc, char* argv[])
{
	using std::chrono::system_clock;
	
	std::chrono::duration<int, std::ratio<60 * 60 * 24> > one_day(1);
	
	system_clock::time_point today = system_clock::now();
	system_clock::time_point tomorrow = today + one_day;

	if (tomorrow > today)
		std::cout << "haha" << std::endl;
	
	std::time_t tt;
	char buffer[1024];
	tt = system_clock::to_time_t(today);
	ctime_s(buffer, 1024, &tt);
	std::cout << "today is: " << buffer;
	tt = system_clock::to_time_t(tomorrow);
	ctime_s(buffer, 1024, &tt);
	std::cout << "tomorrow will be: " << buffer;



	GateServer* server = GateServer::GetInstance();
	if (server->Start(argc, argv))
		server->Run();

#ifdef _WIN32
	system("pause");
#endif
	return 0;
}