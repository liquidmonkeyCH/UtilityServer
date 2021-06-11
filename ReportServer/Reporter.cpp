/**
* @file Reporter.cpp
*
* @author Hourui (liquidmonkey)
**/

#include "Reporter.hpp"
#include "ReportServer.hpp"
#include "Common/json.h"
#include "ServiceReportManager.hpp"

#include "Utility/com_aes.hpp"
#include "Utility/com_base64.hpp"
#include "Utility/com_sha256.hpp"
#include "Common/CommonDefines.hpp"

namespace UProject
{
	size_t response(void* ptr, size_t size, size_t nmemb, void* pReporter) {
		size *= nmemb;
		((Reporter*)pReporter)->on_response((char*)ptr, size);
		return size;
	}

	void Reporter::init(void) {
		m_respones = m_buffer;
		m_http_req.set_response(response, this);
		auto pConf = AppConfig::GetInstance();
		m_key = pConf->m_key.c_str();
		m_appId = pConf->m_appId.c_str();
		m_bizId = pConf->m_bizId.c_str();
	}

	void Reporter::on_response(const char* p, size_t size) {
		memcpy(m_respones, p, size);
		m_respones += size;
		p += size;
		p += *p == '\r' ? 2 : 1;
		if (*p != '0') return;
		Json::Reader Reader;
		Json::Value res,response;
		Reader.parse(m_buffer, m_respones, response);
		m_respones = m_buffer;
		int err = response["errcode"].asInt();
		if (err == 0) {
			return;
		}
		
		if (err <= 2000) {
			Clog::error("report error:%s(%d)", response["errmsg"].asCString(),err);
			return;
		}

		ServiceReportManager* pService = GetService<ServiceReportManager>();
		Clog::warn("report fail:%s(%d)", response["errmsg"].asCString(), err);
		res = response["data"]["results"];
		int no = 0;
		auto* data = m_collection->m_data;
		for (Json::Value::ArrayIndex i = 0; i < res.size(); ++i) {
			no = res[i]["no"].asInt() - 1;
			err = res[i]["errcode"].asInt();
			Clog::info("report fail info: no = %d %s(%d)", no, res[i]["errmsg"].asCString(), err);
			if (err == 3005) {
				data[no].ot = time(nullptr);			// 修改时间后再上报
				pService->insert(data + no);
			}
		}
	}

	static constexpr const char* di_fmt = "{\"no\":%d,\"si\":\"%s\",\"bt\":%d,\"ot\":%lld,\"ct\":%d,\"pi\":null,\"di\":\"%s\"},";
	static constexpr const char* pi_fmt = "{\"no\":%d,\"si\":\"%s\",\"bt\":%d,\"ot\":%lld,\"ct\":%d,\"pi\":\"%s\",\"di\":null},";
	static constexpr const char* co_head = "{\"collections\":[";
	static constexpr const char* co_tail = "]}";
	static constexpr const char* boddy_head = "{\"data\":\"";
	static constexpr const char* boddy_tail = "\"}";

	void Reporter::report(ReportCollection* pCollection) {
		if (nullptr == m_key) init();
		m_collection = pCollection;
		auto timestamps = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		char buffer[MAX_BUFFER_LEN];
		auto* data = pCollection->m_data;
		char* p_out = m_buffer + 16;
		memcpy(m_buffer, co_head, 16);
		size_t len = 0;
		for (int i = 0; i < pCollection->m_size; ++i,++data) {
			len = data->ct == 0 ? snprintf(buffer, MAX_BUFFER_LEN, pi_fmt, i + 1, data->si, data->bt, (long long)data->ot, data->ct, data->pi) :
				snprintf(buffer, MAX_BUFFER_LEN, di_fmt, i + 1, data->si, data->bt, (long long)data->ot, data->ct, data->di);
			memcpy(p_out, buffer, len);
			p_out += len;
		}
		memcpy(p_out - 1, co_tail, 3);

		size_t out_size = MAX_BUFFER_LEN;
		com::hex_to_bin(m_key, buffer, 32);
		com::aes128 aes128;
		aes128.set_key(buffer);
		auto iv = aes128.get_iv();
		aes128.encrypt(out_size, buffer, m_buffer, strlen(m_buffer), com::aes128::mode_t::GCM, com::aes128::padding_t::None);
		p_out = m_buffer;
		memcpy(p_out, iv.m_data, 12);
		p_out += 12;
		memcpy(p_out, buffer, out_size);

		com::base64 base64;
		out_size = base64.encoding(m_buffer, out_size + 12, buffer, MAX_BUFFER_LEN);

		p_out = m_buffer;
		memcpy(p_out, boddy_head, 9);
		p_out += 9;
		memcpy(p_out, buffer, out_size);
		p_out += out_size;
		memcpy(p_out, boddy_tail, 3);
		p_out = m_buffer;

		m_http_req.append_header("Content-Type:application/json;charset=utf-8");
		snprintf(buffer, MAX_BUFFER_LEN, "appId:%s", m_appId);
		m_http_req.append_header(buffer);
		snprintf(buffer, MAX_BUFFER_LEN, "bizId:%s", m_bizId);
		m_http_req.append_header(buffer);
		snprintf(buffer, MAX_BUFFER_LEN, "timestamps:%lld", (long long)timestamps);
		m_http_req.append_header(buffer);
		snprintf(buffer, MAX_BUFFER_LEN, "%sappId%sbizId%stimestamps%lld%s", m_key, m_appId, m_bizId, (long long)timestamps, p_out);
		com::sha256 sha256(buffer, strlen(buffer));
		snprintf(buffer, MAX_BUFFER_LEN, "%s:%s", "sign", sha256.c_str());
		m_http_req.append_header(buffer);
		ServiceReportManager* pService = GetService<ServiceReportManager>();

		if (!m_http_req.post("http://api2.wlc.nppa.gov.cn/behavior/collection/loginout", m_buffer))
			pService->retry(pCollection);
		else
			pService->free(pCollection);

		m_respones = m_buffer;
		m_collection = nullptr;
	}
}
