/**
* @file GateServer.cpp
*
* @author Hourui (liquidmonkey)
**/

#include "GateServer.hpp"

// ϵͳ����
#include "Common/LogSystem.hpp"


namespace UProject
{
	bool GateServer::OnStart(void) { return true; }
	void GateServer::OnStop(void) {}
	// ��ʼ��
	void GateServer::init(void) {}
	// ���ط���������
	bool GateServer::loadConfig(void) { return true; }

	// Log
	void GateServer::createLogsystem(void) {}
	void GateServer::destoryLogsystem(void) {}

	// �������
	void GateServer::createNetwork(void) {}
	void GateServer::destoryNetwork(void) {}

	// �߼�����
	void GateServer::createService(void) {}
	void GateServer::destoryService(void) {}
}
