/**
* @file Defines.hpp
*
* @author Hourui (liquidmonkey)
**/

#ifndef __DEFINES_HPP__
#define __DEFINES_HPP__

#include "Common/CommonDefines.hpp"
#include <time.h>

namespace UProject
{
	static constexpr int MAX_AI_LEN = 32;
	static constexpr int MAX_NAME_LEN = 32;


	static constexpr int MAX_COLLECTIONS = 128;
	static constexpr int MAX_SI_LEN = 32;
	static constexpr int MAX_DI_LEN = 32;
	static constexpr int MAX_PI_LEN = 38;

	struct Data
	{
		char si[MAX_SI_LEN + 2];
		int	bt;
		time_t ot;
		int ct;
		char di[MAX_DI_LEN + 2];
		char pi[MAX_PI_LEN + 2];
	};
}

#endif // __DEFINES_HPP__