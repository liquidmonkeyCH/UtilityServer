/**
* @file Reporter.hpp
*
* @author Hourui (liquidmonkey)
**/

#ifndef __REPORTER_HPP__
#define __REPORTER_HPP__

#include "http_requester.hpp"
#include "StructsDn.hpp"

namespace UProject
{

class Reporter
{
	static constexpr size_t MAX_BUFFER_LEN = 30720;
public:
	Reporter(void) = default;
	~Reporter(void) = default;

	void init(void);
	void report(ReportCollection* pCollection);
	void on_response(const char* p, size_t size);
private:
	http_requester m_http_req;
	char m_buffer[MAX_BUFFER_LEN];
	ReportCollection* m_collection = nullptr;
	char* m_respones = nullptr;
	const char* m_key = nullptr;
	const char* m_appId = nullptr;
	const char* m_bizId = nullptr;
};

}
#endif //!__REPORTER_HPP__
