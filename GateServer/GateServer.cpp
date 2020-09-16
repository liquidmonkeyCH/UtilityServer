/**
* @file GateServer.cpp
*
* @author Hourui (liquidmonkey)
**/

#include "GateServer.hpp"

// 系统服务
#include "Common/LogSystem.hpp"


namespace UProject
{
	bool GateServer::OnStart(void) { return true; }
	void GateServer::OnStop(void) {}
	// 初始化
	void GateServer::init(void) {}
	// 加载服务器配置
	bool GateServer::loadConfig(void) { return true; }

	// Log
	void GateServer::createLogsystem(void) {}
	void GateServer::destoryLogsystem(void) {}

	// 网络服务
	void GateServer::createNetwork(void) {}
	void GateServer::destoryNetwork(void) {}

	// 逻辑服务
	void GateServer::createService(void) {}
	void GateServer::destoryService(void) {}
}
