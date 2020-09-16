/**
* @file task_dispatcher.hpp
*
* @author Hourui (liquidmonkey)
*/
#ifndef __MSG_DISPATCHER_HPP__
#define __MSG_DISPATCHER_HPP__

#include "com_thread_pool.hpp"
#include "msg_channel.hpp"

namespace Utility
{
////////////////////////////////////////////////////////////////////////////////////////////////////
namespace msg
{
////////////////////////////////////////////////////////////////////////////////////////////////////
class controler_iface;
////////////////////////////////////////////////////////////////////////////////////////////////////
class dispatcher
{
public:
	struct task_info
	{
		channel_node* m_obj;
		controler_iface* m_controler;
		void run(void);
	};
	friend class controler_iface;
public:
	dispatcher(void);
	~dispatcher(void);

	dispatcher(const dispatcher&) = delete;
	dispatcher& operator=(const dispatcher&) = delete;

	void start(std::uint32_t nworker=1);
	void stop(void);
private:
	void dispatch(task_info&& _task);
private:
	com::task_threadpool<task_info> m_workers;
};
////////////////////////////////////////////////////////////////////////////////////////////////////
}//namespace msg
////////////////////////////////////////////////////////////////////////////////////////////////////
}//namespace Utility 
#endif //__MSG_DISPATCHER_HPP__