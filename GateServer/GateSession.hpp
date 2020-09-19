/**
* @file GateSession.hpp
*
* @author Hourui (liquidmonkey)
**/

#ifndef __GATE_SESSION_HPP__
#define __GATE_SESSION_HPP__

#include "Common/CommonDefines.hpp"
#include "Utility/msg_pares_len.hpp"
#include "Utility/mem_stream_buffer.hpp"
#include "Utility/com_protobuf.hpp"

namespace UProject
{
	class GateSession : public com::protobuf_session<
		net::socket_type::tcp,msg::pares_len::comfirmer<
		mem::stream_buffer<MAX_PACKET_LEN>>>
	{
	};
}

#endif // !__GATE_SESSION_HPP__

