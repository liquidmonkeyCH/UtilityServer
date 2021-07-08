/**
* @file GateResponder.hpp
*
* @author Hourui (liquidmonkey)
**/

#ifndef __GATE_RESPONDER_HPP__
#define __GATE_RESPONDER_HPP__

#include "Common/CommonDefines.hpp"
#include "Utility/net_responder.hpp"
#include "Utility/msg_handler_manager_deque.hpp"
#include "Utility/msg_pares_len.hpp"
#include "Utility/mem_stream_buffer.hpp"
#include "Utility/com_protobuf.hpp"

namespace UProject
{
	using GateSession = net::tcp::session<msg::pares_len::comfirmer32<msg::buffer<mem::stream_buffer<MAX_PACKET_LEN>>>>;
	class GateResponder : public net::responder<GateSession, msg::handler_manager_alone>
	{
	public:
		GateResponder(void) {}
		~GateResponder(void) {}
	private:
		void on_start(void);
	};
}// namespace CrossGate

#endif //!__GATE_RESPONDER_HPP__