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
		char si[MAX_SI_LEN + 2];		// �Ự��ʶ
		int	bt;							// ��Ϸ�û���Ϊ����
		time_t ot;						// ʱ�������λ��
		int ct;							// �û����� 0������֤ͨ���û� 2���ο��û�
		char di[MAX_DI_LEN + 2];		// �ο�ģʽ�豸��ʶ
		char pi[MAX_PI_LEN + 2];		// ��֤�û���Ψһ��ʶ
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