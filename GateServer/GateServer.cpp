/**
* @file GateServer.cpp
*
* @author Hourui (liquidmonkey)
**/

#include "GateServer.hpp"

#include "Common/LogSystem.hpp"
#include "GateResponder.hpp"

// ϵͳ����
#include "Common/LogSystem.hpp"
#include "UtilityTest.hpp"


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
	// ��ʼ��
	void GateServer::init(void) {}
	// ���ط���������
	bool GateServer::loadConfig(void) { return true; }

	// Log
	void GateServer::createLogsystem(void){
		LogSystem* pLog = LogSystem::GetInstance();
		pLog->Start(LogSystem::log_level::debug);
		//Clog::active_logger(pLog);
	}
	void GateServer::destoryLogsystem(void){
		LogSystem::GetInstance()->Stop();
	}

	// �������
	void GateServer::createNetwork(void){
		m_io_service.start();
		m_dispatcher_client.start();
		m_dispatcher_other.start(10);
		ServiceManager::Attach<GateResponder>(&GateResponder::init, 1000, &m_io_service, &m_dispatcher_client);
		ServiceManager::GetService<GateResponder>()->start("0.0.0.0", 5001); // ��ʱ������ fix me!
	}
	void GateServer::destoryNetwork(void){
		ServiceManager::Detach<GateResponder>(&GateResponder::stop);
		m_io_service.stop();
		m_dispatcher_client.stop();
		m_dispatcher_other.stop();
	}

	// �߼�����
	void GateServer::createService(void) {
		ServiceManager::Attach<UtilityTest>(&UtilityTest::run);
	}
	void GateServer::destoryService(void) {}
}
