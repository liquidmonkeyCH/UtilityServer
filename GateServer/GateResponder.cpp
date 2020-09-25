/**
* @file GateResponder.cpp
*
* @author Hourui (liquidmonkey)
**/

#include "GateResponder.hpp"

namespace UProject
{
	msg::err GateHandler(msg::object_iface* obj, mem::message* msg)
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

		return msg::err::none;
	}

	void GateResponder::on_start(void)
	{
		m_controler.attach(GateHandler);
	}

}// namespace CrossGate
