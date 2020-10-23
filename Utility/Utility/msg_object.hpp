/**
* @file msg_object.hpp
*
* @author Hourui (liquidmonkey)
*/
#ifndef __MSG_OBJECT_HPP__
#define __MSG_OBJECT_HPP__

#include <atomic>
#include <cstdint>
#include <mutex>

#include "msg_channel.hpp"

namespace Utility
{
////////////////////////////////////////////////////////////////////////////////////////////////////
namespace msg
{
////////////////////////////////////////////////////////////////////////////////////////////////////
class dispatcher;
class controler_iface;
////////////////////////////////////////////////////////////////////////////////////////////////////
class object_iface : public channel_node
{
public:
	object_iface(void) : channel_node(false) {};
	virtual ~object_iface(void) = default;

	object_iface(const object_iface&) = delete;
	object_iface& operator=(const object_iface&) = delete;

	virtual void handle_error(int) = 0;
	virtual void do_close(void) = 0;
	virtual mem::message* get_message(void) = 0;
protected:
	controler_iface* m_controler;
};
////////////////////////////////////////////////////////////////////////////////////////////////////
}//namespace msg
////////////////////////////////////////////////////////////////////////////////////////////////////
}//namespace Utility 
#endif //__MSG_OBJECT_HPP__