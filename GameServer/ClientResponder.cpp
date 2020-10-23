/**
* @file ClientResponder.cpp
*
* @author Hourui (liquidmonkey)
**/

#include "ClientResponder.hpp"

namespace UProject
{
	int GateHandler(msg::object_iface* obj, mem::message* msg)
	{
		uint32 id;
		if (!msg->get(id)) return msg::err::unpack;
		switch (id)
		{
		case 100:
			break;
		default:
			break;
		}

		return 0;
	}

	void ClientResponder::on_start(void)
	{
		m_controler.attach(GateHandler);
	}

}// namespace CrossGate