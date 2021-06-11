/**
* @file ServiceDataManager.cpp
*
* @author Hourui (liquidmonkey)
**/

#include "ServiceReportManager.hpp"

namespace UProject
{
	void ServiceReportManager::init(void) {
		bool exp = false;
		if (!m_runnring.compare_exchange_strong(exp, true)) return;
		m_recv = m_data_pool.malloc();
		m_works.init(10);
		m_scheduler = std::thread(&ServiceReportManager::run, this);
	}

	void ServiceReportManager::stop(void) {
		bool exp = true;
		if (!m_runnring.compare_exchange_strong(exp, false)) return;
		s_cv.notify_one();
		m_scheduler.join();
		m_works.safe_stop();
	}

	void ServiceReportManager::run(void) {
		while (m_runnring) {
			std::unique_lock<std::mutex> lock(s_mutex);
			s_cv.wait_for(lock, std::chrono::seconds(1));
		}
	}

	void ServiceReportManager::flush(bool force) {
		do{
			std::lock_guard<std::recursive_mutex> lock(mutex);
			if (m_recv->m_size == 0) break;					// 无新数据进入
			m_sendlist[!m_flag].push_back(m_recv);			// 加入待发送列表
			m_recv = m_data_pool.malloc();
			if (force) {
				if(m_count < 10) s_cv.notify_one();
				return;
			}
		} while (false);

		time_t now = time(nullptr);
		if (m_last_send + 1 < now) {
			m_count = 0;
			m_last_send = now;
		}
		bool swap = false;
		do{
			while (!m_sendlist[m_flag].empty()) {
				m_works.schedule_normal(&ServiceReportManager::report, this, m_sendlist[m_flag].front());
				m_sendlist[m_flag].pop_front();
				if (++m_count == 10) return;
			}

			if (swap) return;
			swap = true;
			std::lock_guard<std::recursive_mutex> lock(mutex);
			m_flag = !m_flag;
		} while (true);
	}

	void ServiceReportManager::report(ReportCollection* p) {
		Reporter* pReporter = m_reporter_pool.malloc();
		pReporter->report(p);
		m_reporter_pool.free(pReporter);
	}

	void ServiceReportManager::insert(ReportData* p) {
		std::lock_guard<std::recursive_mutex> lock(mutex);
		memcpy(m_recv->m_data + m_recv->m_size, p, sizeof(ReportData));
		if (++m_recv->m_size == MAX_COLLECTIONS)
			flush();
	}

	void ServiceReportManager::retry(ReportCollection* p) {
		std::lock_guard<std::recursive_mutex> lock(mutex);
		m_sendlist[!m_flag].push_back(p);
	}
}
