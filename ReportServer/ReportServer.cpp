/**
* @file ReportServer.cpp
*
* @author Hourui (liquidmonkey)
**/

#include "ReportServer.hpp"
#include "Common/LogSystem.hpp"
#include "Common/JsonConfig.hpp"

// 系统服务
#include "Utility/logsystem.hpp"
#include "ServiceReportManager.hpp"
#include "Reporter.hpp"


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
	void ReportServer::OnStop(void) {
		try {
			destoryNetwork();
			destoryService();
			destoryLogsystem();
		}
		catch (std::exception& e) {
			Clog::error(e.what());
		}
	}
	// 初始化
	void ReportServer::init(void) {
		get_param_num();
		loadConfig();
	}
	// 加载服务器配置
	bool ReportServer::loadConfig(void) {
		auto Loader = JsonConfig::GetInstance();
		bool ret = Loader->Load("./ServerConf.json");
		if (!ret) {
			Clog::error_throw(errors::logic,"load ServerConf fail!");
			return false;
		}
		auto pConfig = AppConfig::GetInstance();
		pConfig->m_key = Loader->m_root["secretKey"].asCString();
		pConfig->m_appId = Loader->m_root["appId"].asCString();
		pConfig->m_bizId = Loader->m_root["bizId"].asCString();

		return true; 
	}

	// Log
	void ReportServer::createLogsystem(void) {
		auto pLog = main::logsystem<1024>::GetInstance();
		pLog->start(main::logsystem<1024>::log_level::debug,"",1024);
		Clog::active_logger(pLog);
	}
	void ReportServer::destoryLogsystem(void) {
		LogSystem::GetInstance()->Stop();
	}

	// 网络服务
	void ReportServer::createNetwork(void) {
		m_io_service.start();
		m_dispatcher.start(10);

		/*ServiceManager::Attach<GateResponder>(&GateResponder::init, 1000, &m_io_service, &m_dispatcher_client);
		ServiceManager::GetService<GateResponder>()->start("0.0.0.0", 5001); // 临时放这里 fix me!*/
	}
	void ReportServer::destoryNetwork(void) {
		/*ServiceManager::Detach<GateResponder>(&GateResponder::stop);*/
		m_io_service.stop();
		m_dispatcher.stop();
	}

	// 逻辑服务
	void ReportServer::createService(void) {
		ServiceManager::Attach<ServiceReportManager>(&ServiceReportManager::init);
		/*Reporter kReporter;
		ReportCollection kCollection;
		kCollection.m_size = 2;
		auto* p = kCollection.m_data;
		memcpy(p[0].si, "w7ligxjw355ftctm94yqt9dcew4zd724", MAX_SI_LEN + 1);
		p[0].bt = 0; //上线
		p[0].ot = time(nullptr) - 10;
		p[0].ct = 0;
		p[0].pi[0] = 0;
		//memcpy(p[0].pi, "1hbp4cfvmr8unvaxboe76y8bcaz7q51fime8my", MAX_PI_LEN + 1);
		p[0].di[0] = 0;

		memcpy(p[1].si, "w7ligxjw355ftctm94yqt9dcew4zd723", MAX_SI_LEN + 1);
		p[1].bt = 0; //上线
		p[1].ot = time(nullptr) - 10;
		p[1].ct = 0;
		p[1].pi[0] = 0;
		memcpy(p[1].pi, "1hbp4cfvmr8unvaxboe76y8bcaz7q51fime8my", MAX_PI_LEN + 1);
		p[1].di[0] = 0;

		kReporter.report(&kCollection);*/
	}
	void ReportServer::destoryService(void) {
		ServiceManager::Detach<ServiceReportManager>(&ServiceReportManager::stop);
	}
}
