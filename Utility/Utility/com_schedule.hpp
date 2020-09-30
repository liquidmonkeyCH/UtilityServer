/**
* @file com_schedule.hpp
*
* @author Hourui (liquidmonkey)
*/
#ifndef __COM_SCHEDULE_HPP__
#define __COM_SCHEDULE_HPP__

#include "com_thread_pool.hpp"
#include <chrono>
#include <map>

namespace Utility
{
////////////////////////////////////////////////////////////////////////////////////////////////////
namespace com
{
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T, class _Clock, class _Duration = typename _Clock::duration>
class schedule_wrap
{
protected:
	using func_t = std::function < void() >;
	using time_point = std::chrono::time_point<_Clock,_Duration>;
	using task_t = typename std::conditional<std::is_same<T, func_t>::value, T, task_wrap<T>>::type;
	using threadpool_t = typename std::conditional<std::is_same<T, func_t>::value, com::threadpool, com::task_threadpool<T>>::type;
	using map_t = std::multimap<time_point, task_t>;
	using iterator = typename map_t::iterator;
	enum class state :unsigned char { none, start, stop };
	struct event_t{
		sint64 m_siCode;
		iterator m_iter;
	};
protected:
	threadpool_t m_threadpool;
	std::thread m_main_thread;
	std::multimap<time_point, task_t> tasks;
	std::mutex mtx;
	std::condition_variable cv;
	std::atomic_size_t m_ntasks = { 0 };
	state m_state = state::none;
	sint64 m_siCode;
public:
	schedule_wrap(void) {}
	~schedule_wrap(void) {}
public:
	void init(size_t size)
	{
		assert(size != 0);
		std::unique_lock<std::mutex> lock(this->mtx);
		if (this->m_state != base::state::none) return;
		m_threadpool.init(size);
		this->m_state = base::state::start;
		lock.unlock()
		m_main_thread = std::thread(&schedule_wrap<T>::run_task,this);
	}

	int attach(time_point tp, task_t&& task)
	{
		std::unique_lock<std::mutex> lock(mtx);
		if (m_state != state::start)
			return false;

		tasks.emplace(std::make_pair(tp, task))
			lock.unlock();

		++m_ntasks;
		cv.notify_one();
		return true;
	}

	bool schedule(time_point tp,task_t&& task)
	{
		std::unique_lock<std::mutex> lock(mtx);
		if (m_state != state::start)
			return false;

		tasks.emplace(std::make_pair(tp,task))
		lock.unlock();

		++m_ntasks;
		cv.notify_one();
		return true;
	}

	template<class F, class... Args>
	// �ύ���� �����Ƿ��ύ�ɹ�
	bool schedule_normal(time_point tp, F&& f, Args&&... args)
	{
		return schedule(tp,func_t(std::bind(std::forward<F>(f), std::forward<Args>(args)...)));
	}

	// �ر������ύ �ȴ�ʣ��������� (����ʱ��ȷ��pool��������)
	bool stop(void)
	{
		std::unique_lock<std::mutex> lock(mtx);
		if (m_state != state::start) return false;
		m_state = state::stop;
		if (tasks.empty())
		{
			lock.unlock();
			_destory();
			return true;
		}

		auto task = std::make_shared<std::packaged_task<bool()>>([]() { return true; });
		tasks.emplace([task]() { (*task)(); });

		lock.unlock();

		++m_ntasks;
		cv.notify_one();

		task->get_future().get();
		_destory();

		return true;
	}

	size_t task_size(void) { return m_ntasks; }
private:
	void run_task(void)
	{
		do {
			std::unique_lock<std::mutex> lock(mtx);
			while (tasks.empty())
				cv.wait(lock);

			if (m_state != state::start)
				return;

			auto iter = tasks.begin();
			if (iter == tasks.end())
				continue;

			time_point now = time_point::now();
			cv.wait_until(lock, iter->first)

			task_t task(std::move(tasks.front()));
			tasks.pop();

			lock.unlock();

			--m_ntasks;
			return task;
		} while (true);
	}

	void _run(void)
	{
		++m_working;
		while (true)
		{
			--m_working;
			if (task_t task = get_task())
				task();
			else
				break;
		}
	}
protected:
	void _init(void)
	{
		for (size_t i = 0; i < m_size; ++i)
			pool[i] = std::thread(&thread_pool_wrap::_run, this);

		std::lock_guard<std::mutex> lock(mtx);
		m_state = state::start;
	}

	void _destory(void)
	{
		std::unique_lock<std::mutex> lock(mtx);
		if (m_state == state::none)
			return;

		m_state = state::none;
		lock.unlock();

		cv.notify_all();

		for (size_t i = 0; i < m_size; ++i)
			pool[i].join();

	}
};
////////////////////////////////////////////////////////////////////////////////////////////////////
}// namspace com
////////////////////////////////////////////////////////////////////////////////////////////////////
}// namespace Utility
#endif //__COM_THREAD_POOL_HPP__ 