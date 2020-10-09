/**
* @file com_schedule.hpp
*
* @author Hourui (liquidmonkey)
*/
#ifndef __COM_SCHEDULE_HPP__
#define __COM_SCHEDULE_HPP__

#include "com_thread_pool.hpp"
#include "mem_data_factory.hpp"
#include <chrono>
#include <atomic>
#include <map>

namespace Utility
{
////////////////////////////////////////////////////////////////////////////////////////////////////
namespace com
{
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class _Clock, class _Duration = typename _Clock::duration>
class schedule
{
private:
	using time_point = std::chrono::time_point<_Clock, _Duration>;
	using func_t = std::function<void()>;
	enum class state :unsigned char { none, start, stop };
	struct event_t
	{
		event_t(void) = default;
		~event_t(void) = default;
		std::atomic<std::uint32_t> m_siCode;
		func_t m_func;
	};
public:
	class Event {
	public:
		friend class schedule;
		Event(std::uint32_t siCode, event_t* ev) :m_siCode(siCode), m_event(ev) {}
		~Event(void) = default;
		bool cancel(void) { return m_event ? m_event->m_siCode.compare_exchange_strong(m_siCode, 0) : false; }
		operator bool (void){ return m_event ? m_event->m_siCode == m_siCode : false; }
	private:
		std::uint32_t m_siCode;
		event_t* m_event;
	};
private:
	com::threadpool m_threadpool;
	std::thread m_main_thread;
	std::multimap<time_point, Event> tasks;
	mem::data_factory_ex<event_t, 50> m_event_pool;
	std::mutex mtx;
	std::condition_variable cv;
	std::atomic_size_t m_ntasks = { 0 };
	state m_state = state::none;
	std::uint32_t m_siCode;
public:
	schedule(void) {}
	~schedule(void) {}
public:
	void init(size_t size)
	{
		assert(size != 0);
		std::unique_lock<std::mutex> lock(this->mtx);
		if (this->m_state != state::none) return;
		m_threadpool.init(size);
		this->m_state = state::start;
		lock.unlock();
		m_main_thread = std::thread(&schedule<_Clock,_Duration>::run_task,this);
	}

	template<class F, class... Args>
	Event attach(time_point tp, F&& f, Args&&... args)
	{
		std::unique_lock<std::mutex> lock(mtx);
		if (m_state != state::start)
			return {0,nullptr};

		if (++m_siCode == 0)
			m_siCode = 1;
		event_t* ev = m_event_pool.malloc();
		ev->m_siCode = m_siCode;
		ev->m_func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);
		auto iter = tasks.emplace(std::make_pair(tp, Event{ m_siCode, ev }));

		++m_ntasks;
		cv.notify_one();
		return iter->second;
	}

	// 关闭任务提交 等待剩余任务完成 (调用时请确保pool不会析构)
	bool stop(void)
	{
		std::unique_lock<std::mutex> lock(mtx);
		if (m_state != state::start) return false;
		m_state = state::stop;
		m_threadpool.safe_stop();
		lock.unlock();
		cv.notify_one();
		m_main_thread.join();
		return true;
	}

	size_t task_size(void) { return m_ntasks; }
private:
	void run_task(void)
	{
		do {
			std::unique_lock<std::mutex> lock(this->mtx);
			if (tasks.empty())
				cv.wait(lock);

			if (m_state != state::start)
				return;

			auto begin = tasks.begin();
			cv.wait_until(lock, begin->first);

			if (m_state != state::start)
				return;

			time_point now = std::chrono::system_clock::now();
			auto iter = begin;
			do {
				if (iter->second.cancel())
					m_threadpool.schedule(std::move(iter->second.m_event->m_func));
				m_event_pool.free(iter->second.m_event);
				--m_ntasks;
			} while (++iter != tasks.end() && iter->first <= now);
			tasks.erase(begin,iter);
		} while (true);
	}
};
////////////////////////////////////////////////////////////////////////////////////////////////////
}// namspace com
////////////////////////////////////////////////////////////////////////////////////////////////////
}// namespace Utility
#endif //__COM_THREAD_POOL_HPP__ 