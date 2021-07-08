/**
* @file ClientResponder.hpp
*
* @author Hourui (liquidmonkey)
**/

#ifndef __CLIENT_RESPONDER_HPP__
#define __CLIENT_RESPONDER_HPP__

#include "Common/CommonDefines.hpp"
#include "Utility/net_responder.hpp"
#include "Utility/msg_handler_manager_deque.hpp"
#include "Utility/msg_pares_len.hpp"
#include "Utility/mem_stream_buffer.hpp"
#include "Utility/com_protobuf.hpp"

namespace UProject
{
	using ClientSession = net::tcp::session<msg::pares_len::comfirmer32<msg::buffer<mem::stream_buffer<MAX_PACKET_LEN>>>>;
	class ClientResponder : public net::responder<ClientSession, msg::handler_manager_alone>
	{
	public:
		ClientResponder(void) {}
		~ClientResponder(void) {}
	private:
		void on_start(void);
	};
}// namespace CrossGate

#endif //!__CLIENT_RESPONDER_HPP__