/**
* @file User.hpp
*
* @author Hourui (liquidmonkey)
**/

#ifndef __USER_HPP__
#define __USER_HPP__

#include "Common/CommonDefines.hpp"
#include "Utility/msg_session.hpp"
#include "Utility/msg_pares_len.hpp"
#include "Utility/mem_rotative_buffer.hpp"

namespace UProject
{
	template<typename ...Args>
	class User_Component{};
	template<typename T,typename ...Tails>
	class User_Component<T,Tails...> : public T, public User_Component<Tails...> {};
	template<typename T>
	class User_Component<T> : public T{};
	
	class User_BasicInfo {};
	class User_SceneInfo {};

	class CUser : public User_Component
		<User_BasicInfo
		,User_SceneInfo
	>{};
}

#endif // !__USER_HPP__
