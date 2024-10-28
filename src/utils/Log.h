/**
*  Copyright (c) 2019, 亿力吉奥
*  All rights reserved.
*
*  @file        log.h
*  @author      huqi
*  @date        2019-09
*
*  @brief		日志
*  @note        无
*  @version
*/
#pragma once

//#include <json.hpp>
#include <queue>
#include <set>
#include <memory>
#include <mutex>

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/fmt/fmt.h>
#include <unordered_map>
#include <functional>


namespace BBGLOBE {

	enum class EPLanguage {
		English = 0,
		Chinese = 1
	};

	enum class EPType {
		Daily = 0,
		Base = 1
	};
	/**
	* \brief	日志
	* \author	huqi
	* \date		2019-09
	*/
	class Log
	{
	public:
		static Log& getInstance();
		void initLog(const std::string& logName = "", const std::string& fileName = "",const EPType eptype = EPType::Daily, const bool isMt = true,bool writeDebugToFile = false);
		void removeLog(const std::string& logName);
		template<typename T>
		void debug(const T& str_u8);
		template<typename T>
		void info(const T& str_u8);
		template<typename T>
		void warn(const T& str_u8);
		template<typename T>
		void error(const T& str_u8);

		template<typename... Args>
		void debug(const char *fmt_u8, const Args &... args);
		template<typename... Args>
		void info(const char *fmt_u8, const Args &... args);
		template<typename... Args>
		void warn(const char *fmt_u8, const Args &... args);
		template<typename... Args>
		void error(const char *fmt_u8, const Args &... args);

		std::queue<std::pair<std::string, std::string>> GetLastErrorMsg();
		void SetLanguage(EPLanguage language = EPLanguage::English);
		void ClearErrorMsg();
		void OutToStdErr(const std::string& key, const std::string& info);
		void OutLastErrToSeverStdErr(const std::string& key="");

		std::string Translate(const std::string& msg);

		template<typename... Args>
		std::string CombineStr(const char *fmt_u8, const Args &... args);

		//断言
		void Assert(bool b);
	private:
		std::string Translate(const char* msg);
		void StashError(const std::string& fmt_u8, const std::string& trans_u8="");

		void ExportLogInfo(const char * fmt_u8, std::function<void(std::shared_ptr<spdlog::logger>, const std::string&)> customfunc);
		bool TranslateBase(const char * msg, std::function<void(const std::string&, const std::string&)> fun);

	private:
		Log();
		~Log();
		Log(const Log&);
		Log& operator=(const Log&);
		std::set<std::shared_ptr<spdlog::logger> > m_loglst;
		std::mutex m_queueMtx;
		std::mutex m_initMtx;
		std::queue<std::pair<std::string,std::string>> m_lastErrorMsg;
		EPLanguage m_language;

		//nlohmann::json m_translateJson;
		//std::unordered_map<std::string, nlohmann::json*> m_fastTranslationHashMap;
	};
}

template<typename T>
void BBGLOBE::Log::debug(const T& str_u8)
{
	ExportLogInfo(str_u8.c_str(),[](std::shared_ptr<spdlog::logger> log, const std::string& str) {
		log->debug(str.c_str());
	});
}

template<typename T>
void BBGLOBE::Log::info(const T& str_u8)
{
	ExportLogInfo(str_u8.c_str(), [](std::shared_ptr<spdlog::logger> log, const std::string& str) {
		log->info(str.c_str());
	});
}

template<typename T>
void BBGLOBE::Log::warn(const T& str_u8)
{
	ExportLogInfo(str_u8.c_str(), [](std::shared_ptr<spdlog::logger> log, const std::string& str) {
		log->warn(str.c_str());
	});
}

template<typename T>
void BBGLOBE::Log::error(const T& str_u8)
{
	ExportLogInfo(str_u8.c_str(), [](std::shared_ptr<spdlog::logger> log, const std::string& str) {
		log->error(str.c_str());
	});
	
	StashError(str_u8,str_u8);
}

template<typename ...Args>
void BBGLOBE::Log::debug(const char * fmt_u8, const Args & ...args)
{
	std::string msg_u8 = CombineStr(fmt_u8, args...);

	ExportLogInfo(msg_u8.c_str(), [&](std::shared_ptr<spdlog::logger> log, const std::string& str) {
		log->debug(str.c_str());
	});

}
template<typename ...Args>
void BBGLOBE::Log::info(const char * fmt_u8, const Args & ...args)
{
	std::string msg_u8 = CombineStr(fmt_u8, args...);

	ExportLogInfo(msg_u8.c_str(), [&](std::shared_ptr<spdlog::logger> log, const std::string& str) {
		log->info(str.c_str());
	});
}
template<typename ...Args>
void BBGLOBE::Log::warn(const char * fmt_u8, const Args & ...args)
{
	std::string msg_u8 = CombineStr(fmt_u8, args...);

	ExportLogInfo(msg_u8.c_str(), [&](std::shared_ptr<spdlog::logger> log, const std::string& str) {
		log->warn(str.c_str());
	});
}
template<typename ...Args>
void BBGLOBE::Log::error(const char * fmt_u8, const Args & ...args)
{
	std::string msg_u8 = CombineStr(fmt_u8, args...);

	ExportLogInfo(msg_u8.c_str(), [&](std::shared_ptr<spdlog::logger> log, const std::string& str) {
		log->error(str.c_str());
	});

	StashError(std::string(fmt_u8),msg_u8);
}

template<typename ...Args>
inline std::string BBGLOBE::Log::CombineStr(const char * fmt_u8, const Args & ...args)
{
	std::string fmt1_u8 = Translate(fmt_u8);
	fmt::memory_buffer buf;
	fmt::format_to(buf, fmt1_u8.c_str(), args...);
	return std::string(buf.data(), buf.size());
}

std::string Tr(const std::string& msg);
#define LogIns BBGLOBE::Log::getInstance()

