/**
* @file application.cpp
*
* @author Hourui (liquidmonkey)
**/

#include "Utility/application.hpp"
#include "Utility/logger.hpp"

#include <iostream>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef _WIN32
#include <unistd.h>
#endif

#ifdef _WIN32
#include <Windows.h>
#endif

namespace Utility
{
////////////////////////////////////////////////////////////////////////////////
namespace main
{
////////////////////////////////////////////////////////////////////////////////
void
application::controler::yield(void)
{
	std::unique_lock<std::mutex> lock(m_mutex);
	m_cv.wait(lock);
}
////////////////////////////////////////////////////////////////////////////////
void
application::controler::resume(void)
{
	m_cv.notify_all();
}
////////////////////////////////////////////////////////////////////////////////
bool
application::controler::start(void)
{
	state exp = state::none;
	if (m_state.compare_exchange_strong(exp, state::running))
		return true;
	return false;
}
////////////////////////////////////////////////////////////////////////////////
void
application::controler::stop(void)
{
	state exp = state::running;
	if (m_state.compare_exchange_strong(exp, state::stopping))
		resume();
}
////////////////////////////////////////////////////////////////////////////////
void
application::controler::run(void)
{
	while (m_state != state::stopping)
		yield();

	m_state = state::none;
}
////////////////////////////////////////////////////////////////////////////////
application::controler*
application::get_controler(void)
{
	static controler m_controler;
	return &m_controler;
}
////////////////////////////////////////////////////////////////////////////////
bool
application::Start(int param_num, char* params[])
{
	if (!get_controler()->start())
		return false;
	bool bDaemon = false;
	for (int i = 0; i < param_num; ++i)
	{
		if (strcmp(params[i], "-d") == 0 || strcmp(params[i], "-daemon") == 0)
			bDaemon = true;

		m_param_list.push_back(params[i]);
	}

	if (bDaemon)
		daemon();

	setsignal();
	return OnStart();
}
////////////////////////////////////////////////////////////////////////////////
void
application::Run(void)
{
	Clog::info("Application start success!");
	get_controler()->run();
	OnStop();
}
////////////////////////////////////////////////////////////////////////////////
void
application::daemon(void)
{
#ifndef _WIN32
	int pid = fork();
	if (pid != 0)
	{
		// parent exit
		_exit(-1);
	}
	// create a new session
	setsid();

	fclose(stdin);
	fclose(stdout);
	fclose(stderr);
#endif // !_WIN32
}
////////////////////////////////////////////////////////////////////////////////
void on_signal(int n)
{
	if (n == SIGINT || n == SIGTERM)
	{
		application::get_controler()->stop();
		return;
	}
}
////////////////////////////////////////////////////////////////////////////////
void
application::setsignal(void)
{
#ifndef _WIN32
	signal(SIGHUP, SIG_IGN);
	signal(SIGPIPE, SIG_IGN);
	struct sigaction act;
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_NODEFER | SA_ONSTACK | SA_RESETHAND;
	act.sa_handler = on_signal;
	sigaction(SIGINT, &act, NULL);
	sigaction(SIGTERM, &act, NULL);
#else
	signal(SIGINT, on_signal);
	signal(SIGTERM, on_signal);
#endif // !_WIN32
}
////////////////////////////////////////////////////////////////////////////////
void application::yeild(void)
{
	application::get_controler()->yield();
}
////////////////////////////////////////////////////////////////////////////////
void application::resume(void)
{
	application::get_controler()->resume();
}
////////////////////////////////////////////////////////////////////////////////
size_t
application::get_param_num(void)
{
	return m_param_list.size();
}
////////////////////////////////////////////////////////////////////////////////
const char*
application::get_param(size_t n)
{
	if (n >= m_param_list.size())
		return nullptr;

	return m_param_list[n].c_str();
}
////////////////////////////////////////////////////////////////////////////////
}// namespace main
////////////////////////////////////////////////////////////////////////////////
} //namespace Utility
