/**
* @file JsonConfig.hpp
*
* @author laijia
**/

#ifndef __JSON_CONFIG_HPP__
#define __JSON_CONFIG_HPP__

#include "CommonDefines.hpp"
#include "Utility/com_singleton.hpp"
#include "json.h"

namespace UProject
{
	class JsonConfig : public com::iface::Singleton<JsonConfig>
	{
		DECLARE_SINGLETON_DEFAULT(JsonConfig)
	public:
		bool Load(const char* json_file);

		Json::Value m_root;
		Json::Value m_kGameConf;
		Json::Value m_kWorldConf;
		Json::Value m_kMsgConf;
		Json::Value m_kDBConf;
		Json::Value m_kGameCliConf;
		Json::Value m_kMysqlConf;
	};
}

#endif // !__JSON_CONFIG_HPP__