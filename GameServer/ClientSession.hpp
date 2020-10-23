/**
* @file ClientSession.hpp
*
* @author Hourui (liquidmonkey)
**/

#ifndef __CLIENT_SESSION_HPP__
#define __CLIENT_SESSION_HPP__

#include "Common/CommonDefines.hpp"
#include "Utility/msg_pares_len.hpp"
#include "Utility/mem_stream_buffer.hpp"
#include "Utility/com_protobuf.hpp"

namespace UProject
{
	class ClientSession : public com::protobuf_session<
		net::socket_type::tcp, msg::pares_len::comfirmer<
		mem::stream_buffer<MAX_PACKET_LEN>>>
	{
	public:
		ClientSession(void) {}
		~ClientSession(void) = default;
	private:
		void on_connect(void) {}
		void on_close(reason) {}
	private:
	};
}

#endif // !__CLIENT_SESSION_HPP__

