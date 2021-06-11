/**
* @file http_requester.cpp
*
* @author Hourui (liquidmonkey)
**/

#include "http_requester.hpp"
#include "Common/CommonDefines.hpp"

namespace UProject
{
    size_t req_reply(void* ptr, size_t size, size_t nmemb, void* stream)
    {
        std::string* str = (std::string*)stream;
        //cout << *str << endl;
        (*str).append((char*)ptr, size * nmemb);
        std::cout << str->c_str() << std::endl;
        std::cout << std::this_thread::get_id << std::endl;
        return size * nmemb;
    }

    http_requester::~http_requester(void) {
        if (curl) {
           curl_easy_cleanup(curl);
           curl = nullptr;
        }
    }

    void http_requester::init(void) {
        if (!curl) {
            curl = curl_easy_init();
            curl_easy_setopt(curl, CURLOPT_HEADER, 0);
            curl_easy_setopt(curl, CURLOPT_POST, 1);
            curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5);
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5);
        }
    }

	bool http_requester::post(const char* URL,const char* data) {
        int res = 0;
        if (!curl) init();
        
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, m_headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);    // 指定post内容        
        curl_easy_setopt(curl, CURLOPT_URL, URL); // 指定url

        res = (int)curl_easy_perform(curl);
        free_headers();
        
        if (res != 0) {
            Clog::error("curl post error:%d!", res);
            return false;
        }
        return true;
	}
}
