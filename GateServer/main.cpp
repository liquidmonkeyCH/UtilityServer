/**
* @file main.cpp
*
* @author Hourui (liquidmonkey)
**/

#include "GateServer.hpp"
#include <Chrono>
#include "Utility/com_scheduler.hpp"
#include "Utility/logger.hpp"
using namespace UProject;

int main(int argc, char* argv[])
{
	logger kLogger(logger::log_level::debug);
	Clog::active_logger(&kLogger);

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

	com::scheduler<std::chrono::system_clock> m_scheduler;

	m_scheduler.init(10);
	m_scheduler.attach(system_clock::now() + std::chrono::microseconds(2000), []() {Clog::debug("haha1"); });
	m_scheduler.attach(system_clock::now() + std::chrono::microseconds(2100), []() {Clog::debug("haha2"); });
	m_scheduler.attach(system_clock::now() + std::chrono::microseconds(2500), []() {Clog::debug("haha3"); });
	auto res = m_scheduler.attach(system_clock::now() + std::chrono::microseconds(2700), []() {Clog::debug("haha4"); });
	m_scheduler.attach(system_clock::now() + std::chrono::microseconds(4000), []() {Clog::debug("hoho"); });
	std::this_thread::sleep_for(std::chrono::microseconds(1400));
	if (res.cancel()) Clog::debug("Cancel,%lld",system_clock::now().time_since_epoch().count());
	m_scheduler.stop();
	res = m_scheduler.attach(system_clock::now() + std::chrono::microseconds(2700), []() {Clog::debug("haha5"); });
	if (!res) Clog::debug("Bad event");
	GateServer* server = GateServer::GetInstance();
	if (server->Start(argc, argv))
		server->Run();

#ifdef _WIN32
	system("pause");
#endif
	return 0;
}