/**
* @file GateServer.cpp
*
* @author Hourui (liquidmonkey)
**/

#include "GateServer.hpp"

#include "GateResponder.hpp"

// 系统服务
#include "UtilityTest.hpp"
#include "Utility/logsystem.hpp"


namespace UProject
{
	bool GateServer::OnStart(void) { 
		try {
			init();
			createLogsystem();
			createNetwork();
			createService();
		}
		catch(std::exception& e){
			Clog::error(e.what());
			return false;
		}
		return true; 
	}
	void GateServer::OnStop(void) {}
	// 初始化
	void GateServer::init(void) {}
	// 加载服务器配置
	bool GateServer::loadConfig(void) { return true; }

	// Log
	void GateServer::createLogsystem(void){
		main::logsystem* pLog = main::logsystem::GetInstance();
		pLog->start("./GateServer", main::logsystem::level::debug);
		Clog::active_logger(pLog);
	}
	void GateServer::destoryLogsystem(void){
		main::logsystem::GetInstance()->stop();
	}

	// 网络服务
	void GateServer::createNetwork(void){
		m_io_service.start();
		m_dispatcher_client.start();
		m_dispatcher_other.start(10);
		ServiceManager::Attach<GateResponder>(&GateResponder::init, 1000, &m_io_service, &m_dispatcher_client);
		ServiceManager::GetService<GateResponder>()->start("0.0.0.0", 5001); // 临时放这里 fix me!
	}
	void GateServer::destoryNetwork(void){
		ServiceManager::Detach<GateResponder>(&GateResponder::stop);
		m_io_service.stop();
		m_dispatcher_client.stop();
		m_dispatcher_other.stop();
	}

	// 逻辑服务
	void GateServer::createService(void) {
		ServiceManager::Attach<UtilityTest>(&UtilityTest::run);
	}
	void GateServer::destoryService(void) {}
}
