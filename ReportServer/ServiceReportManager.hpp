/**
* @file ServiceReportManager.hpp
*
* @author Hourui (liquidmonkey)
**/

#ifndef __SERVICE_REPORT_MANAGER_HPP__
#define __SERVICE_REPORT_MANAGER_HPP__

#include "Common/CommonDefines.hpp"
#include "Utility/mem_data_factory.hpp"
#include "Reporter.hpp"

namespace UProject
{
	class ServiceReportManager : public com::iface::Service
	{
		DECLARE_SERVICE_DEFAULT(ServiceReportManager);
	public:
		void init(void);
		void stop(void);

		void free(ReportCollection* p) { p->m_size = 0; m_data_pool.free(p); }
		void insert(ReportData* p);
		void retry(ReportCollection* p);
	private:
		void run(void);
		void flush(bool force = true);
		void report(ReportCollection* p);
	private:
		std::recursive_mutex mutex;
		com::threadpool m_works;
		mem::data_pool_s<Reporter, 10> m_reporter_pool;
		mem::data_pool_s<ReportCollection, 10> m_data_pool;
		ReportCollection* m_recv;
		std::list<ReportCollection*> m_sendlist[2];
		bool m_flag = false;
		time_t m_last_send = 0;
		size_t m_count = 0;
		
		std::thread m_scheduler;
		std::mutex s_mutex;
		std::condition_variable s_cv;
		std::atomic_bool m_runnring = false;
	};
}
#endif // !__SERVICE_DATA_MANAGER_HPP__

