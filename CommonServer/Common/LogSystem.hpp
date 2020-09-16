/**
* @file LogSystem.hpp
*
* @author Hourui (liquidmonkey)
**/

#ifndef __LOG_SYSTEM_HPP__
#define __LOG_SYSTEM_HPP__

#include "CommonDefines.hpp"
#include "Utility/mem_data_factory.hpp"
#include "Utility/com_thread_pool.hpp"
#include "Utility/com_singleton.hpp"
#include "Utility/com_guard.hpp"

namespace CrossGate
{
	class LogRecorder
	{
	private:
		static constexpr int LEVEL_LEN = 7;			// [ERROR][WARN][INFO][DEBUG]
		static constexpr int DATE_LEN = 21;			// [YYYY-MM-DD HH:MM:SS]
		static constexpr int THREAD_ID_LEN = 10;	// [FFFFFFFF]
		static constexpr int HEAD_LEN = LEVEL_LEN + DATE_LEN + THREAD_ID_LEN;
		// [DEBUG][YYYY-MM-DD HH:MM:SS][FFFFFFFF]LOG_STR\n
		static constexpr int BLOCK_SIZE = HEAD_LEN + Clog::MAX_LOG_LEN;
		static constexpr int MAX_RECORD_LEN = BLOCK_SIZE * 10;

		struct node_t
		{
			node_t(void);
			~node_t(void) = default;
			inline void clear(void)
			{
				m_pReader = m_szBuffer;
				m_pWirter = m_szBuffer;
				m_pNext = nullptr;
			}

			char m_szBuffer[MAX_RECORD_LEN + BLOCK_SIZE];
			const char* m_pTail;
			char* m_pReader;
			char* m_pWirter;
			node_t* m_pNext;
		};

		friend class LogSystem;
	public:
		LogRecorder(void);
		~LogRecorder(void) = default;

		const char* Read(std::size_t& size);
		void CommitRead(void);
		inline char* Wirte(void) const { return m_pWirterNode->m_pWirter; }
		bool CommitWirte(std::size_t size);
	private:
		std::mutex m_mutex;
		mem::data_pool<node_t,5, 5, mem::factory_cache_type::DYNAMIC> m_kPool;
		node_t* m_pReaderNode;
		node_t* m_pWirterNode;
		std::ptrdiff_t m_nLastRead;

		LogRecorder* m_pIdlePrev;
	}; // class LogRecorder

	class LogSystem : public logger_iface, public com::iface::Singleton<LogSystem>
	{
	public:
		enum class state_t { none, running, stopping };
		enum class log_level {
			error = 0,
			warn = 1,
			info = 2,
			debug = 3
		};
		struct task_info
		{
			LogSystem* m_pService;
			LogRecorder* m_pRecoder;
			void run(void) { m_pService->save(m_pRecoder); }
		};
		DECLARE_SINGLETON(LogSystem)
	public:
		void Start(log_level);
		void Stop(void);
	private:
		void save(LogRecorder* pRecorder);
	public:
		void debug(const char*);
		void info(const char*);
		void warn(const char*);
		void error(const char*);
	private:
		std::mutex m_mutex;
		com::task_thread<task_info> m_thread;
		mem::data_factory_ex<LogRecorder, 5> m_kPool;
		std::atomic<state_t> m_state;
		log_level m_level;
		LogRecorder* m_pIdleRecorder;

		std::ofstream m_kFile;
	}; // class LogSystem
}// namespace CrossGate
#endif // __LOG_SYSTEM_HPP__

