/**
* @file GateServer.hpp
*
* @author Hourui (liquidmonkey)
**/

#ifndef __GATE_SERVER_HPP__
#define __GATE_SERVER_HPP__


#include "Common/CommonDefines.hpp"
#include "Utility/application.hpp"
#include "Utility/com_singleton.hpp"
#include "Utility/net_io_service_epoll.hpp"
#include "Utility/msg_dispatcher.hpp"

namespace UProject
{
	class GateServer : public main::application, public com::iface::Singleton<GateServer>
	{
		DECLARE_SINGLETON_DEFAULT(GateServer)
	public:
		bool OnStart(void);
		void OnStop(void);
	private:
		// ��ʼ��
		void init(void);
		// ���ط���������
		bool loadConfig(void);

		// Log
		void createLogsystem(void);
		void destoryLogsystem(void);

		// �������
		void createNetwork(void);
		void destoryNetwork(void);

		// �߼�����
		void createService(void);
		void destoryService(void);
	private:
		net::io_service_epoll m_io_service;
		msg::dispatcher m_dispatcher_client;
		msg::dispatcher m_dispatcher_other;
	};
}

#endif // !__GATE_SERVER_HPP__


