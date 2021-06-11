/**
* @file Defines.hpp
*
* @author Hourui (liquidmonkey)
**/

#ifndef __DEFINES_HPP__
#define __DEFINES_HPP__

#include <stdlib.h>
#include <stdio.h>
#include <cstddef>
#include <ctime>
#include <atomic>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <memory>
#include <string>
#include <string.h>
#include <deque>
#include <queue>
#include <list>
#include <vector>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <algorithm>
#include <functional>
#include <math.h>
#include <fstream>
#include <streambuf>
#include <iostream>
#include <sstream>
#include <chrono>
#include <cstdio>

#ifdef _WIN32
inline tm* localtime_r(const time_t* timep, struct tm* result)
{
	localtime_s(result, timep);
	return result;
}
#else
#include <pthread.h>
inline pthread_t GetCurrentThreadId()
{
	return pthread_self();
}
#endif // _WIN32
#include "Utility/com_service.hpp"
#include "Utility/com_service_manager.hpp"
#include "Utility/logger.hpp"
#include "Utility/mem_message.hpp"
#include "Utility/msg_object.hpp"
using namespace Utility;

namespace UProject
{
	using schar8 = std::int8_t;
	using uchar8 = std::uint8_t;
	using sint16 = std::int16_t;
	using uint16 = std::uint16_t;
	using sint32 = std::int32_t;
	using uint32 = std::uint32_t;
	using sint64 = std::int64_t;
	using uint64 = std::uint64_t;

	static constexpr size_t MAX_PACKET_LEN = 4096;

	class CData
	{
	public:
		CData() :m_uiLoadFlag(0), m_uiDirtyFlag(0) {}
	protected:
		uint32 m_uiLoadFlag;
		uint32 m_uiDirtyFlag;
	};

	template<class T>
	using Service = com::wrap::Service<T>;
	using ServiceManager = com::ServiceManager;
	template<class T>
	static auto GetService = ServiceManager::GetService<T>;

#ifdef NDEBUG
#define FUNCTION_LOG()
#define HANDLE_LOG(_obj)
#else
#define FUNCTION_LOG()\
		com::guard<const char*> __FUNCTION_LOG(__FUNCTION__,\
			[](const char* func){Clog::debug("Enter %s", func);},\
			[](const char* func){Clog::debug("Leave %s", func);});
#define HANDLE_LOG(_obj)\
		com::guard<const char*> __FUNCTION_LOG(__FUNCTION__,\
			[=](const char* func){Clog::debug("[%p]Enter %s",_obj, func);},\
			[=](const char* func){Clog::debug("[%p]Leave %s",_obj, func);});
#endif

#define MSG_PROTOBUF_UNPACK(message,buffer)	\
	message k##message;{\
		com::protobuf_istream kStream(buffer);\
		if(!k##message.ParseFromZeroCopyStream(&kStream))	\
			return msg::err::unpack;	\
	}

#define MSG_ROTATIVE_UNPACK(message,buffer) \
	message* p##message;{\
		net_size_t size = sizeof(message);\
		buffer->reset();\
		p##message = (message*)buffer->next(size);\
	}

#define MSG_STREAM_UNPACK(message,buffer) \
	message k##message;{\
		buffer->reset();\
		net_size_t size = sizeof(message);\
		const char* p;\
		net_size_t len = size;\
		net_size_t pos = 0;\
		do{\
			p = msg->next(len);\
			if (!p) return msg::err::unpack;\
			memcpy((char*)(&k##message) + pos, p, len);\
			pos += len;\
			if (pos >= size) break;\
			len = size - pos;\
		} while (true);\
	}
}

#endif //__DEFINES_HPP__


