/**
* @file LogSystem.cpp
*
* @author Hourui (liquidmonkey)
**/

#include "LogSystem.hpp"
#ifdef  _WIN32
#include <Windows.h>
#endif //  _WIN32
#include <iostream>

#define SERVICE_LOG_OUT(type,str)					\
	if(m_level < log_level::type) return;			\
	LogRecorder* pRecorder;							\
	std::unique_lock<std::mutex> lock(m_mutex);		\
	if (nullptr == m_pIdleRecorder) {				\
		pRecorder = m_kPool.malloc();				\
	}else{											\
		pRecorder = m_pIdleRecorder;				\
		m_pIdleRecorder = pRecorder->m_pIdlePrev;	\
	}												\
	lock.unlock();									\
	tm tmNow;										\
	time_t tNow = time(NULL);						\
	localtime_r(&tNow, &tmNow);						\
	char* pBuffer = pRecorder->Wirte();				\
	snprintf(pBuffer,LogRecorder::BLOCK_SIZE		\
		, "[%s][%04d-%02d-%02d %02d:%02d:%02d][%08lX]%s\n", #type	\
		, tmNow.tm_year + 1900, tmNow.tm_mon + 1, tmNow.tm_mday		\
		, tmNow.tm_hour, tmNow.tm_min, tmNow.tm_sec					\
		, ::GetCurrentThreadId(), str);								\
	std::size_t len = strlen(pBuffer);								\
	pBuffer[len] = 0x0;												\
	if(pRecorder->CommitWirte(len))									\
		m_thread.schedule(task_info{this,pRecorder});				\
	lock.lock();													\
	pRecorder->m_pIdlePrev = m_pIdleRecorder;						\
	m_pIdleRecorder = pRecorder;

namespace CrossGate
{
	
	LogRecorder::node_t::node_t(void)
		: m_szBuffer{ 0 }
		, m_pTail(m_szBuffer + MAX_RECORD_LEN)
		, m_pReader(m_szBuffer)
		, m_pWirter(m_szBuffer)
		, m_pNext(nullptr)
	{
	}
	
	LogRecorder::LogRecorder(void)
		: m_pReaderNode(m_kPool.malloc())
		, m_pWirterNode(m_pReaderNode)
		, m_nLastRead(0)
		, m_pIdlePrev(nullptr)
	{
	}
	
	const char* LogRecorder::Read(std::size_t& size)
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		if (m_pReaderNode != m_pWirterNode)
			lock.unlock();

		m_nLastRead = m_pReaderNode->m_pWirter - m_pReaderNode->m_pReader;
		size = m_nLastRead;
		return m_pReaderNode->m_pReader;
	}
	
	void LogRecorder::CommitRead(void)
	{
		m_pReaderNode->m_pReader += m_nLastRead;
		std::lock_guard<std::mutex> lock(m_mutex);
		if (m_pReaderNode->m_pReader > m_pReaderNode->m_pTail)
		{
			node_t* pNode = m_pReaderNode;
			m_pReaderNode = m_pReaderNode->m_pNext;
			pNode->clear();
			m_kPool.free(pNode);
		}
	}
	
	bool LogRecorder::CommitWirte(std::size_t size)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_pWirterNode->m_pWirter += size;
		if (m_pWirterNode->m_pWirter > m_pWirterNode->m_pTail)
		{
			m_pWirterNode->m_pNext = m_kPool.malloc();
			m_pWirterNode = m_pWirterNode->m_pNext;
		}

		return 0 == m_nLastRead;
	}

	//	ServiceLogSystem
	LogSystem::LogSystem(void)
		: m_state(state_t::none)
		, m_level(log_level::debug)
		, m_pIdleRecorder(nullptr)
	{
	}
	
	LogSystem::~LogSystem(void)
	{
		m_thread.safe_stop();
	}
	
	void LogSystem::Start(log_level lv)
	{
		state_t kExp = state_t::none;
		if (!m_state.compare_exchange_strong(kExp, state_t::running))
			Clog::error_throw(errors::logic, "ServiceLogSystem already running!");

		m_level = lv;
	}

	void LogSystem::Stop(void)
	{
		state_t kExp = state_t::running;
		if (!m_state.compare_exchange_strong(kExp, state_t::stopping))
			return;

		m_thread.safe_stop();
		m_state = state_t::none;
	}

	void LogSystem::save(LogRecorder* pRecorder)
	{
		std::size_t nSize;
		const char* p;
		do {
			p = pRecorder->Read(nSize);
			if (0 == nSize)
				return;

			// log save
			std::cout << p;

			

			pRecorder->CommitRead();
		} while (true);
	}
	
	void LogSystem::debug(const char* p){ SERVICE_LOG_OUT(debug, p); }
	void LogSystem::info(const char* p) { SERVICE_LOG_OUT(info, p); }
	void LogSystem::warn(const char* p) { SERVICE_LOG_OUT(warn, p); }
	void LogSystem::error(const char* p) { SERVICE_LOG_OUT(error, p); }
}// namespace CrossGate
