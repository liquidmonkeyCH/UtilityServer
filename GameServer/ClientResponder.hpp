/**
* @file ClientResponder.hpp
*
* @author Hourui (liquidmonkey)
**/

#ifndef __CLIENT_RESPONDER_HPP__
#define __CLIENT_RESPONDER_HPP__

#include "Utility/net_responder.hpp"
#include "Utility/msg_handler_manager_deque.hpp"
#include "ClientSession.hpp"

namespace UProject
{
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