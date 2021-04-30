#include "UtilityTest.hpp"
#include "Utility/logger.hpp"
#include <iostream>
#include <sstream>
#include <fstream>
using namespace Utility;

#include "Utility/com_hex_caster.hpp"
void com_hex_caster(void)
{
	Clog::debug("");
	Clog::debug("");
	Clog::debug("/////////////////////////////////////////////////////////////////////////");
	Clog::debug("// com::hex_caster");
	Clog::debug("/////////////////////////////////////////////////////////////////////////");

	Clog::info("data(char[16]): 0,1,2,3,4,5,36,7,8,9,215,11,12,13,14,255, ");
	unsigned char data[16] = { 0,1,2,3,4,5,36,7,8,9,215,11,12,13,14,255 };
	auto mm = com::bin2hex<true>(data);
	Clog::debug("bin2hex:%s", mm.str());

	unsigned char res[16];
	com::hex2bin<true>(res, mm.str(), mm.len);
	std::stringstream aa("");
	for (int i = 0; i < 16; ++i)
		aa << (unsigned int)res[i] << ",";
	Clog::debug("hex2bin(char[16]):%s",aa.str().c_str());
}

#include "Utility/com_md5.hpp"
void com_md5(void)
{
	Clog::debug("");
	Clog::debug("");
	Clog::debug("/////////////////////////////////////////////////////////////////////////");
	Clog::debug("// com::md5");
	Clog::debug("/////////////////////////////////////////////////////////////////////////");

	Clog::info("data(char[16]): 0,1,2,3,4,5,36,7,8,9,215,11,12,13,14,255, ");
	unsigned char data[16] = { 0,1,2,3,4,5,36,7,8,9,215,11,12,13,14,255 };
	com::md5 md5_data(data, 16);

	Clog::debug("md5:%s", md5_data.c_str());

	Clog::info("data1(char[8]): 0,1,2,3,4,5,36,7, ");
	Clog::info("data2(char[8]): 8,9,215,11,12,13,14,255, ");
	unsigned char data1[8] = { 0,1,2,3,4,5,36,7 };
	unsigned char data2[8] = { 8,9,215,11,12,13,14,255 };
	md5_data.reset();
	md5_data.update(data1, 8);

	Clog::debug("data1->md5:%s", md5_data.c_str());

	md5_data.update(data2, 8);

	Clog::debug("data1+data2->md5:%s", md5_data.c_str());

	md5_data.reset();
	md5_data.update("12345", 5);
	Clog::debug("12345->md5:%s", md5_data.c_str());
}

#include "Utility/com_guard.hpp"
void com_guard(void)
{
	Clog::debug("");
	Clog::debug("");
	Clog::debug("/////////////////////////////////////////////////////////////////////////");
	Clog::debug("// com::guard");
	Clog::debug("/////////////////////////////////////////////////////////////////////////");

	com::guard<const char*> kGuard("hahaha", 
		[](const char* p) { Clog::debug("%s Enter", p); },
		[](const char* p) { Clog::debug("%s Leave", p); });
}

#include "Utility/com_scheduler.hpp"
#include <chrono>
void com_scheduler(void)
{
	Clog::debug("");
	Clog::debug("");
	Clog::debug("/////////////////////////////////////////////////////////////////////////");
	Clog::debug("// com::scheduler");
	Clog::debug("/////////////////////////////////////////////////////////////////////////");

	com::scheduler<std::chrono::system_clock> m_scheduler;

	m_scheduler.init(10);
	m_scheduler.attach(std::chrono::microseconds(2000), []() {Clog::debug("haha0"); });
	m_scheduler.attach(std::chrono::microseconds(4000), []() {Clog::debug("hoho"); });
	m_scheduler.attach(std::chrono::microseconds(2000), []() {Clog::debug("haha1"); });
	m_scheduler.attach(std::chrono::microseconds(2100), []() {Clog::debug("haha2"); });
	m_scheduler.attach(std::chrono::microseconds(2500), []() {Clog::debug("haha3"); });
	auto res = m_scheduler.attach(std::chrono::microseconds(2700), []() {Clog::debug("haha4"); });
	std::this_thread::sleep_for(std::chrono::microseconds(1400));
	if (res.cancel()) Clog::debug("Cancel,%lld", std::chrono::system_clock::now().time_since_epoch().count());
	m_scheduler.stop();
	res = m_scheduler.attach(std::chrono::microseconds(2700), []() {Clog::debug("haha5"); });
	if (!res) Clog::debug("Bad event");
}

#include "Utility/task_dispatcher.hpp"
#include "Utility/task_controler.hpp"
#include "Utility/task_object.hpp"
void task_object(void) {
	task::dispatcher m_dispatcher;
	task::controler m_controler;
	m_dispatcher.start(10);
	m_controler.init(&m_dispatcher);

	task::object obj1;
	task::object_channel channel1;
	task::object_channel channel2;

	obj1.init(&m_controler);
	channel1.init(&m_controler);
	channel2.init(&m_controler);
	
	obj1.exec([]() {std::this_thread::sleep_for(std::chrono::microseconds(100)); std::cout << "obj1.exec 1" << std::endl; });
	obj1.enter_channel(channel1.to_channel());
	channel1.exec([]() {std::this_thread::sleep_for(std::chrono::microseconds(100)); std::cout << "channel1.exec 1" << std::endl; });
	channel1.enter_channel(channel2.to_channel());
	channel2.exec([]() {std::this_thread::sleep_for(std::chrono::microseconds(100)); std::cout << "channel2.exec 1" << std::endl; });
	obj1.exec([&]() { std::cout << "obj1.exec 2"  << std::endl; });
	channel1.exec([&]() { std::cout << "channel1.exec 2" << std::endl; });
	channel2.exec([]() { std::cout << "channel2.exec 2" << std::endl; });
	channel1.close();
	std::this_thread::sleep_for(std::chrono::seconds(10));
	m_dispatcher.stop();
}

void data_pool(void) {
	mem::data_factory<int, 10> m_factory;
	mem::data_pool<int, 10> m_pool;
	mem::data_factory_s<int, 10> m_factory_s;
	mem::data_pool_s<int, 10> m_pool_s;

	int* a = m_factory.malloc();
	m_factory.free(a);

	a = m_pool.malloc();
	m_pool.free(a);

	a = m_factory_s.malloc();
	m_factory_s.free(a);

	a = m_pool_s.malloc();
	m_pool_s.free(a);

}

void UtilityTest::run(void)
{
	com_guard();
	com_hex_caster();
	com_md5();
	com_scheduler();
	task_object();
	data_pool();
}
