/**
* @file ReportServer.hpp
*
* @author Hourui (liquidmonkey)
**/

#ifndef __REPORT_SERVER_HPP__
#define __REPORT_SERVER_HPP__

#include "Common/CommonDefines.hpp"
#include "Utility/application.hpp"
#include "Utility/com_singleton.hpp"
#include "Utility/net_io_service_epoll.hpp"
#include "Utility/task_dispatcher.hpp"

namespace UProject
{
	class ReportServer : public main::application, public com::iface::Singleton<ReportServer>
	{
		DECLARE_SINGLETON_DEFAULT(ReportServer)
	public:
		bool OnStart(void);
		void OnStop(void);
	private:
		// 初始化
		void init(void);
		// 加载服务器配置
		bool loadConfig(void);

		// Log
		void createLogsystem(void);
		void destoryLogsystem(void);

		// 网络服务
		void createNetwork(void);
		void destoryNetwork(void);

		// 逻辑服务
		void createService(void);
		void destoryService(void);
	private:
		net::io_service_epoll m_io_service;
		task::dispatcher m_dispatcher;
	};
}

#endif // !__REPORT_SERVER_HPP__


