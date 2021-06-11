/**
* @file StructsDn.hpp
*
* @author Hourui (liquidmonkey)
**/

#ifndef __STRUCTSDN_HPP__
#define __STRUCTSDN_HPP__

#include "Common/CommonDefines.hpp"
#include "Utility/com_singleton.hpp"
#include <time.h>

namespace UProject
{
	static constexpr int MAX_AI_LEN = 32;
	static constexpr int MAX_NAME_LEN = 32;


	static constexpr int MAX_COLLECTIONS = 128;
	static constexpr int MAX_SI_LEN = 32;
	static constexpr int MAX_DI_LEN = 32;
	static constexpr int MAX_PI_LEN = 38;

	template<class T> using sigleton = com::iface::Singleton<T>;

	struct ReportData
	{
		char si[MAX_SI_LEN + 2];		// 会话标识
		int	bt;							// 游戏用户行为类型
		time_t ot;						// 时间戳，单位秒
		int ct;							// 用户类型 0：已认证通过用户 2：游客用户
		char di[MAX_DI_LEN + 2];		// 游客模式设备标识
		char pi[MAX_PI_LEN + 2];		// 认证用户的唯一标识
		int m_errno = 0;
	};

	struct ReportCollection
	{
		ReportData m_data[MAX_COLLECTIONS];
		int m_size = 0;
	};

	class AppConfig : public com::iface::Singleton<AppConfig>
	{
		DECLARE_SINGLETON_DEFAULT(AppConfig);
	public:
		std::string m_key;
		std::string m_appId;
		std::string m_bizId;
	};
}

#endif // __STRUCTSDN_HPP__