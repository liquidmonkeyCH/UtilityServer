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
	//std::this_thread::sleep_for(std::chrono::seconds(1));
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

#include "Utility/com_widgets.hpp"
void widgets(void) {
	int x = 10;
	int y = 1000;
	auto f = com::future(x);
	std::cout << " now:" << f.set(100) << " 1st:" << f.set(y);
	std::cout << " 2nd:" << x << std::endl;

	struct test {
		void print() { std::cout << "ok" << std::endl; }
	};

	
}

void com_thread(void) {
	com::threadpool_ex pool;
	pool.set_grow(10);
	pool.init(5);
	std::atomic_int num = 0;
	std::atomic_bool running = true;

	pool.schedule([&]() { 
		while (running) {
			std::cout << "suspend:" << pool.suspend_size() << " size:" << pool.size() << std::endl; 
			std::this_thread::sleep_for(std::chrono::microseconds(1000));
		}});

	for(int i = 1; i<50; ++i)
		pool.schedule([&]() { std::this_thread::sleep_for(std::chrono::microseconds(1000)); std::cout << ++num << ":running" << std::endl; });

	std::this_thread::sleep_for(std::chrono::seconds(5));
	running = false;
	pool.safe_stop();
}

#include "Utility/com_character.hpp"
void com_character(void) {
	const char* str = "哈哈哈";
	char buffer[1024];
	com::GbkToUtf8(str, &buffer[0], 1024);
	std::cout << buffer << std::endl;
	com::Utf8ToGbk(buffer, buffer, 1024);
	std::cout << buffer << std::endl;
}

#include "Utility/com_base64.hpp"
void com_base64(void) {
	const char* str = "asdasdasdasd[][][]{}[][";
	char encode[1024];
	char decode[1024];
	com::base64 base64;
	base64.encoding(str, strlen(str), encode, 1024, false);
	std::cout << encode << std::endl;
	base64.decoding(encode, strlen(encode), decode, 1024);
	std::cout << decode << std::endl;
}

#include "Utility/com_sha256.hpp"
void com_sha256(void) {
	Clog::debug("");
	Clog::debug("");
	Clog::debug("/////////////////////////////////////////////////////////////////////////");
	Clog::debug("// com::sha256");
	Clog::debug("/////////////////////////////////////////////////////////////////////////");

	Clog::info("data(char[16]): 0,1,2,3,4,5,36,7,8,9,215,11,12,13,14,255, ");
	unsigned char data[16] = { 0,1,2,3,4,5,36,7,8,9,215,11,12,13,14,255 };
	com::sha256 sha256_data(data, 16);

	Clog::debug("sha256:%s", sha256_data.c_str());

	Clog::info("data1(char[8]): 0,1,2,3,4,5,36,7, ");
	Clog::info("data2(char[8]): 8,9,215,11,12,13,14,255, ");
	unsigned char data1[8] = { 0,1,2,3,4,5,36,7 };
	unsigned char data2[8] = { 8,9,215,11,12,13,14,255 };
	sha256_data.reset();
	sha256_data.update(data1, 8);

	Clog::debug("data1->sha256:%s", sha256_data.c_str());

	sha256_data.update(data2, 8);

	Clog::debug("data1+data2->sha256:%s", sha256_data.c_str());

	sha256_data.reset();
	sha256_data.update("d363289c501dbd9ea253a78b330ea014appId2668efcce0b1414f8f2fa0dd69142dfabizId2201011347timestamps1623059982404", 107);
	sha256_data.update("{\"data\":\"", 9);
	sha256_data.update("MLAzKefArZ3zw48HCaIrdc",22);
	sha256_data.update("+fbGzkY+jodBbUB/", 16);
	sha256_data.update("V0DtSASPotkJnUP7S9J5jcy07uuq/", 29);
	sha256_data.update("QIbecZOL3ivS3sBaYf52nlaC8STNQF3+", 32);
	sha256_data.update("lCgWTP7qSdO8jNSpA0EMHTNo9Z3NVpYibAVEkRthEKCe/", 45);
	sha256_data.update("eBVqhQ/kXCKc0xni/F/", 19);
	sha256_data.update("rMHcBDTCEzSauMYs2W0YBeZrS57qguP3s7R3UUdvKewOknNU0tySUR5l6MlPZ52VOy6VxX2", 71);
	sha256_data.update("/ZMqCB\"}", 8);
	Clog::debug("12345->sha256:%s", sha256_data.c_str());
}

#include "Utility/com_aes.hpp"
inline void Xor(std::uint8_t* mtx, const std::uint8_t* key, size_t n) { for (int i = 0; i < n; ++i) *mtx++ ^= *key++; }
void com_aes(void) {
	char key[] = "aaaaaaaaaaaaaaaa";
	char plain[] = "0123456789abcdef0123456789abcdef0123456789abcdef";
	/* https://www.ssleye.com/aes_aead_cipher.html */
	char iv[] = u8"456789abcdef";
	const char* temp = "10b237e529bcc9f4527086af6604e2a9579ad243ebf5a035bbcec9f861ff5c820fcdc03679895a5f9e9d49124a3a70d7f1271480eec4d4c75ce0f7ff281352e6";
	const char* p = "456789abcdef";

	//Xor((std::uint8_t*)(plain), (std::uint8_t*)p, 16);
	char out[1024];
	char a = 0x44;

	com::aes128 aes128;
	aes128.set_key(key);
	aes128.set_iv(iv,12);
	size_t out_len = 1024;

	aes128.decrypt(out_len, out, plain, strlen(plain), com::aes128::mode_t::GCM, com::aes128::padding_t::Pkcs7);
	std::cout << "加密后大小：" << out_len << std::endl;
	std::cout << "加密后的密文：" << std::endl;
	for (int i = 0; i < out_len; ++i)
	{
		std::cout << std::hex << std::uint32_t((std::uint8_t)out[i]) << " ";
		if ((i + 1) % 4 == 0)
			std::cout << std::endl;
	}
	std::cout << std::endl;
}

#include "curl/curl.h"
void curl_test(void) {
	CURL* curl;
	CURLcode res;
	std::string data;
	struct curl_slist* headers = NULL;
	headers = curl_slist_append(headers, "'Content - Type':'application / json; charset = utf - 8','appId':'09139edbe20f4418b62cdc76d354419c','bizId':'1101999999'");
	curl = curl_easy_init();    // 初始化
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);   // 改协议头
		curl_easy_setopt(curl, CURLOPT_URL, "http://www.baidu.com");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, [](void* ptr, size_t size, size_t len, void* p) {
			((std::string*)p)->append((char*)ptr, size * len); return size * len; });
		//curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);         //将返回的http头输出到fp指向的文件
		res = curl_easy_perform(curl);   // 执行
		if (res != 0) {

			curl_slist_free_all(headers);
			curl_easy_cleanup(curl);
		}

		curl_easy_setopt(curl, CURLOPT_URL, "http://www.baidu.com");
		res = curl_easy_perform(curl);   // 执行
	}
}

template<class T>
class list
{
public:
	class node
	{
		friend class list;
		node* m_prev;
		node* m_next;
	};

	inline void push_back(node* p) {
		++m_size;
		if (m_tail) {
			p->m_prev = m_tail;
			m_tail->m_next = p;
		}
		else
			m_head = m_tail = p;
	}
private:
	node* m_head{ nullptr };
	node* m_tail{ nullptr };
	size_t m_size{ 0 };
};

struct list_node : public list<list_node>::node
{
	int m_prev;
};



void UtilityTest::run(void)
{
	com_guard();
	com_hex_caster();
	com_md5();
	com_scheduler();
	task_object();
	data_pool();
	widgets();
	//com_thread();
	com_character();
	com_base64();
	com_sha256();
	com_aes();
	//curl_test();


	list<list_node> kList;

	list_node A;
	A.m_prev = 1;
	kList.push_back(&A);
}
