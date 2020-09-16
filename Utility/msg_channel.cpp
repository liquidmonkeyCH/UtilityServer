/**
* @file msg_channel.cpp
*
* @author Hourui (liquidmonkey)
*/

#include "Utility/msg_channel.hpp"
#include "Utility/msg_controler.hpp"

namespace Utility
{
////////////////////////////////////////////////////////////////////////////////////////////////////
namespace msg
{
////////////////////////////////////////////////////////////////////////////////////////////////////
void channel_node::clear(void)
{
	m_prev = nullptr;
	m_next = nullptr;
	m_parent = nullptr;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void channel_node::leave_channel(void)
{
	if (m_parent)
		m_parent->detach(this);
}
////////////////////////////////////////////////////////////////////////////////////////////////////
channel::channel(void)
: channel_node(true)
, m_post_root(nullptr)
, m_post_tail(nullptr)
{
}
////////////////////////////////////////////////////////////////////////////////////////////////////
channel::~channel(void)
{
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void channel::attach(channel_node* node)
{
#ifndef NDEBUG
	if (node->m_parent)
		Clog::error_throw(errors::logic, "node already have channel!");
	if(std::this_thread::get_id() != node->m_thread_id)
		Clog::error_throw(errors::logic, "plase executes on its own thread!");
	if(node == this)
		Clog::error_throw(errors::logic, "can not attach self!");
#endif
	node->m_parent = this;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
void channel::detach(channel_node* node)
{
#ifndef NDEBUG
	if (node->m_parent != this)
		Clog::error_throw(errors::logic, "node not belong to this channel!");
#endif
	node->m_parent = nullptr;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
bool channel::post_node(channel_node* node)
{
#ifndef NDEBUG
	if (node->m_parent != this)
		Clog::error_throw(errors::logic, "node is not child!");
#endif
	std::lock_guard<std::mutex> lock(m_mutex);
	if (node == m_post_root) {			// ����Ͷ����Ϣ(��dispatch�߳�Ͷ��)
		if (nullptr == node->m_next)	// Ψһ�ӽڵ�
			return true;

		// ��root�ƶ���tail
		m_post_root = node->m_next;
		m_post_root->m_prev = nullptr;
		node->m_prev = m_post_tail;
		node->m_next = nullptr;
		m_post_tail->m_next = node;
		m_post_tail = node;
		return true;
	}

	// �½��ڵ�
	node->m_prev = m_post_tail;
	node->m_next = nullptr;
	m_post_tail ? m_post_tail->m_next = node : m_post_root = node;
	m_post_tail = node;

	return node == m_post_root;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
channel_node* channel::front(void)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	return m_post_root;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
bool channel::pop_front(void)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	m_post_root = m_post_root->m_next;
	if (m_post_root) {
		m_post_root->m_prev = nullptr;
		return true;
	}
	m_post_tail = nullptr;
	return false;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
}//namespace msg
////////////////////////////////////////////////////////////////////////////////////////////////////
}//namespace Utility 