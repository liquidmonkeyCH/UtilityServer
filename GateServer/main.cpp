/**
* @file main.cpp
*
* @author Hourui (liquidmonkey)
**/

#include "GateServer.hpp"
#include <Chrono>
#include "Utility/com_schedule.hpp"
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

	com::schedule<std::chrono::system_clock> m_scheduler;

	m_scheduler.init(10);
	m_scheduler.attach(system_clock::now() + std::chrono::microseconds(2000), []() {std::cout << "haha1" << std::endl; });
	m_scheduler.attach(system_clock::now() + std::chrono::microseconds(2100), []() {std::cout << "haha2" << std::endl; });
	m_scheduler.attach(system_clock::now() + std::chrono::microseconds(2500), []() {std::cout << "haha3" << std::endl; });
	auto res = m_scheduler.attach(system_clock::now() + std::chrono::microseconds(2700), []() {std::cout << "haha4" << std::endl; });
	m_scheduler.attach(system_clock::now() + std::chrono::microseconds(3000), []() {std::cout << "hoho" << std::endl; });
	std::this_thread::sleep_for(std::chrono::microseconds(2000));
	if (res.cancel()) std::cout << "Cancle" << std::endl;
	m_scheduler.stop();
	
	GateServer* server = GateServer::GetInstance();
	if (server->Start(argc, argv))
		server->Run();

#ifdef _WIN32
	system("pause");
#endif
	return 0;
}