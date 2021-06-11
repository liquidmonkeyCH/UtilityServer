/**
* @file http_requester.hpp
*
* @author Hourui (liquidmonkey)
**/

#ifndef __HTTP_REQUESTER_HPP__
#define __HTTP_REQUESTER_HPP__

#include "curl/curl.h"

namespace UProject
{

class http_requester
{
	using writer_handler = size_t(*)(void*, size_t, size_t, void*);
public:
	http_requester(void) = default;
	~http_requester(void);

	bool post(const char* URL, const char* data);

	void append_header(const char* str) { m_headers = curl_slist_append(m_headers, str); }

	void set_response(writer_handler callback, void* p) {
		if (!curl) init();
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback); 
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, p);
	}
private:
	void free_headers(void) { if (!m_headers) return; curl_slist_free_all(m_headers); m_headers = nullptr; }
	void init(void);
private:
	CURL* curl = nullptr;
	struct curl_slist* m_headers = nullptr;
};

}
#endif
