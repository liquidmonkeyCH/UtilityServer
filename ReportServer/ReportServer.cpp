/**
* @file ReportServer.cpp
*
* @author Hourui (liquidmonkey)
**/

#include "ReportServer.hpp"
#include "Common/LogSystem.hpp"
#include "Common/JsonConfig.hpp"

// ϵͳ����
#include "Common/LogSystem.hpp"

namespace UProject
{
	bool ReportServer::OnStart(void) {
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
	void ReportServer::OnStop(void) {}
	// ��ʼ��
	void ReportServer::init(void) {
		get_param_num();
		loadConfig();
	}
	// ���ط���������
	bool ReportServer::loadConfig(void) {
		bool ret = JsonConfig::GetInstance()->Load("./ServerConf.json");
		if (!ret) {
			Clog::error_throw(errors::logic,"load ServerConf fail!");
			return false;
		}

		

		return true; 
	}

	// Log
	void ReportServer::createLogsystem(void) {
		LogSystem* pLog = LogSystem::GetInstance();
		pLog->Start(LogSystem::log_level::debug);
		//Clog::active_logger(pLog);
	}
	void ReportServer::destoryLogsystem(void) {
		LogSystem::GetInstance()->Stop();
	}

	// �������
	void ReportServer::createNetwork(void) {
		m_io_service.start();
		m_dispatcher.start(10);

		/*ServiceManager::Attach<GateResponder>(&GateResponder::init, 1000, &m_io_service, &m_dispatcher_client);
		ServiceManager::GetService<GateResponder>()->start("0.0.0.0", 5001); // ��ʱ������ fix me!*/
	}
	void ReportServer::destoryNetwork(void) {
		/*ServiceManager::Detach<GateResponder>(&GateResponder::stop);*/
		m_io_service.stop();
		m_dispatcher.stop();
	}

	// �߼�����
	void ReportServer::createService(void) {
	}
	void ReportServer::destoryService(void) {}
}
