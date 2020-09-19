/**
* @file GateResponder.hpp
*
* @author Hourui (liquidmonkey)
**/

#ifndef __GATE_RESPONDER_HPP__
#define __GATE_RESPONDER_HPP__

#include "Utility/net_responder.hpp"
#include "Utility/msg_handler_manager_deque.hpp"
#include "GateSession.hpp"

namespace UProject
{
	class GateResponder : public net::responder<GateSession, msg::hanlder_manager_alone>
	{
	public:
		GateResponder(void) {}
		~GateResponder(void) {}
	private:
		void on_start(void);
	};
}// namespace CrossGate

#endif //!__GATE_RESPONDER_HPP__