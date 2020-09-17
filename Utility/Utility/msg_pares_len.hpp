/**
* @file msg_pares_len.hpp
*
* @author Hourui (liquidmonkey)
*/
#ifndef __MSG_PARES_LEN_HPP__
#define __MSG_PARES_LEN_HPP__

#include "msg_message.hpp"

namespace Utility
{
////////////////////////////////////////////////////////////////////////////////////////////////////
namespace msg
{
////////////////////////////////////////////////////////////////////////////////////////////////////
namespace pares_len
{
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class buffer_type, net_size_t max_message_len = buffer_type::pre_block_size>
class comfirmer : public _message_impl<buffer_type, max_message_len>
{
public:
	using buffer_t = buffer_type;

	comfirmer(void)
	{
		static_assert(max_message_len > sizeof(std::uint32_t), "max_message_len out of range!");
	}
	virtual ~comfirmer(void) = default;
	template<class message_wrap, class handler_manager> friend class msg::controler_wrap;
private:
	state comfirm(void)
	{
		net_size_t size = this->m_size;
		if (size == 0)							// δ��ȡ��Ϣ����
		{
			size = sizeof(std::uint32_t);
			if (!this->readable_state(size))	// ��Ϣ����Ч
				return state::bad;

			if (size == 0)						// ��Ϣ���Ȳ���
				return state::pending;

			size = sizeof(std::uint32_t);
			net_size_t pos = 0;
			const char* p;
			do {
				p = this->next(size);
				memcpy((char*)(&this->m_size) + pos, p, size);
				pos += size;
				if (pos >= sizeof(std::uint32_t)) break;
				size = sizeof(std::uint32_t) - pos;
			} while (true);

			if (this->m_size < sizeof(std::uint32_t) || this->m_size > max_message_len)
				return state::error;

			size = this->m_size;
		}

		if (!this->readable_state(size))	// ��Ϣ����Ч
			return state::bad;

		//  last_read == 0
		if (size == 0)						// ��Ϣ���Ȳ���
			return state::pending;

		this->set_read_limit(this->m_size);	// ��Ϣ���� 
		return state::ok;
	}
};
////////////////////////////////////////////////////////////////////////////////////////////////////
}//namespace pares_len
////////////////////////////////////////////////////////////////////////////////////////////////////
}//namespace msg
////////////////////////////////////////////////////////////////////////////////////////////////////
}//namespace Utility 
#endif //__MSG_MESSAGE_HPP__
