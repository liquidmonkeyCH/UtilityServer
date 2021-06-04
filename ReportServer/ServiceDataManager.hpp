/**
* @file ServiceDataManager.hpp
*
* @author Hourui (liquidmonkey)
**/

#ifndef __SERVICE_DATA_MANAGER_HPP__
#define __SERVICE_DATA_MANAGER_HPP__

#include "Common/CommonDefines.hpp"
#include "Utility/mem_data_factory.hpp"

namespace UProject
{
	class ServiceDataManager : public com::iface::Service
	{
		DECLARE_SERVICE_DEFAULT(ServiceDataManager);

	};
}
#endif // !__SERVICE_DATA_MANAGER_HPP__

