/**
* @file GameServer.cpp
*
* @author Hourui (liquidmonkey)
**/

#include "GameServer.hpp"

#include "Common/LogSystem.hpp"
#include "ClientResponder.hpp"

// ϵͳ����
#include "Common/LogSystem.hpp"


namespace UProject
{
	bool GameServer::OnStart(void) {
		try {
			init();
			createLogsystem();
			createNetwork();
			createService();
		}
		catch (std::exception& e) {
			Clog::error(e.what());
			return false;
		}
		return true;
	}
	void GameServer::OnStop(void) {}
	// ��ʼ��
	void GameServer::init(void) {}
	// ���ط���������
	bool GameServer::loadConfig(void) { return true; }

	// Log
	void GameServer::createLogsystem(void) {
		LogSystem* pLog = LogSystem::GetInstance();
		pLog->Start(LogSystem::log_level::debug);
		//Clog::active_logger(pLog);
	}
	void GameServer::destoryLogsystem(void) {
		LogSystem::GetInstance()->Stop();
	}

	// �������
	void GameServer::createNetwork(void) {
		m_io_service.start();
		m_dispatcher_client.start();
		m_dispatcher_other.start(10);
		ServiceManager::Attach<ClientResponder>(&ClientResponder::init, 1000, &m_io_service, &m_dispatcher_client);
		ServiceManager::GetService<ClientResponder>()->start("0.0.0.0", 5001); // ��ʱ������ fix me!
	}
	void GameServer::destoryNetwork(void) {
		ServiceManager::Detach<ClientResponder>(&ClientResponder::stop);
		m_io_service.stop();
		m_dispatcher_client.stop();
		m_dispatcher_other.stop();
	}

	// �߼�����
	void GameServer::createService(void) {
		
	}
	void GameServer::destoryService(void) {}
}
