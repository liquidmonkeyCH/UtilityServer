/**
* @file com_singleton.hpp
*
* @author Hourui (liquidmonkey)
*/

#ifndef __COM_SINGLETON_HPP__
#define __COM_SINGLETON_HPP__

namespace Utility
{
////////////////////////////////////////////////////////////////////////////////////////////////////
namespace com
{
////////////////////////////////////////////////////////////////////////////////////////////////////
namespace iface
{
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
class Singleton
{
protected:
	Singleton(void) {}
	~Singleton(void) {}

	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
	Singleton(const Singleton&&) = delete;
	Singleton& operator=(const Singleton&&) = delete;
public:
	static T* GetInstance(void)
	{
		static_assert(std::is_base_of<Singleton<T>, T>::value, "need inheritance");
		static T _data;
		return &_data;
	}
};
////////////////////////////////////////////////////////////////////////////////
#define DECLARE_SINGLETON(singletonId)	\
public:									\
	const char* GetID(void)				\
	{									\
		return (#singletonId);			\
	}									\
	static const char* ID(void)			\
	{									\
		return (#singletonId);			\
	}									\
protected:								\
	singletonId(void);					\
	~singletonId(void);					\
	template<class> friend class Utility::com::iface::Singleton;
#define DECLARE_SINGLETON_DEFAULT(singletonId)	\
public:									\
	const char* GetID(void)				\
	{									\
		return (#singletonId);			\
	}									\
	static const char* ID(void)			\
	{									\
		return (#singletonId);			\
	}									\
protected:								\
	singletonId(void) = default;		\
	~singletonId(void) = default;		\
	template<class> friend class Utility::com::iface::Singleton;
////////////////////////////////////////////////////////////////////////////////////////////////////
}
////////////////////////////////////////////////////////////////////////////////////////////////////
namespace wrap
{
////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
class Singleton : public T
{
	Singleton(void) {}
	~Singleton(void) {}


	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
	Singleton(const Singleton&&) = delete;
	Singleton& operator=(const Singleton&&) = delete;
public:
	static Singleton* GetInstance(void)
	{
		static Singleton _data;
		return &_data;
	}
};
////////////////////////////////////////////////////////////////////////////////////////////////////
}// namespace wrap
////////////////////////////////////////////////////////////////////////////////////////////////////
}// namespace com
////////////////////////////////////////////////////////////////////////////////////////////////////
}// namespace Utility
#endif //__COM_SINGLETON_HPP__
