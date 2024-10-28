#include "Log.h"
#include "FileSystemUtil.h"
#include "StringConverter.h"
//#include "JsonUtil.h"

#include <assert.h>
using namespace std;
using namespace BBGLOBE;

std::queue<std::pair<std::string, std::string>> BBGLOBE::Log::GetLastErrorMsg()
{
	std::lock_guard<std::mutex> locl(m_queueMtx);
	return m_lastErrorMsg;
}

void BBGLOBE::Log::SetLanguage(EPLanguage language/*  = EPLanguage::English*/)
{
	m_language = language;
}

void BBGLOBE::Log::ClearErrorMsg()
{
	std::queue<std::pair<std::string, std::string>> empty;
	swap(empty, m_lastErrorMsg);
}

void BBGLOBE::Log::OutToStdErr(const std::string & key, const std::string & info)
{
	std::lock_guard<std::mutex> locl(m_queueMtx);
	LogIns.debug("{} {}", key, info);
//#ifdef _WIN32
//	std::cout << StringConverter::GetTempUseString(key) << StringConverter::GetTempUseString(info) << std::endl;
//#else
	//cout << key << info << endl;
//#endif

}

void BBGLOBE::Log::OutLastErrToSeverStdErr(const std::string& key)
{
	if (LogIns.GetLastErrorMsg().size() > 0)
		LogIns.OutToStdErr(key.empty()?"epgis-error-message:":"", LogIns.GetLastErrorMsg().back().first);
}

std::string BBGLOBE::Log::Translate(const std::string& msg)
{
	return Translate(msg.c_str());
}

std::string BBGLOBE::Log::Translate(const char * msg)
{
	std::string result = "";
	TranslateBase(msg, [&](const std::string& message, const std::string& code){
		result = message;
	});
	return result;
}

bool BBGLOBE::Log::TranslateBase(const char * msg,std::function<void(const std::string&, const std::string&)> fun)
{
	//if (m_language == EPLanguage::English) {
	//	fun(std::string(msg),"");
	//	return true;
	//}

	//目前只有中文英文


	//2.匹配字符串
	//for (auto it = m_translateJson.begin(); it != m_translateJson.end(); ++it) {
	//	nlohmann::json obj = it.value();
	//	if (obj.find("en") != obj.end() && obj["en"].is_string()) {
	//		std::string strEn = obj["en"];
	//		if (strEn == msg) {
	//			if (obj.find("cn") != obj.end() && obj["cn"].is_string()) {
	//				std::string strCn = obj["cn"].get<string>();
	//				return strCn;
	//			}
	//		}
	//	}
	//}

	//auto it = m_fastTranslationHashMap.find(std::string(msg));
	//if (it != m_fastTranslationHashMap.end()) {
	//	nlohmann::json& obj = *(it->second);
	//	fun(JsonUtil::GetString(obj, m_language == EPLanguage::English?"en":"cn"), JsonUtil::GetString(obj, "code", "-1"));
	//	return true;
	//}

	fun(std::string(msg), "");
	return false;
}

void BBGLOBE::Log::StashError(const std::string& fmt_u8, const std::string& trans_u8)
{
	std::lock_guard<std::mutex> lock(m_queueMtx);
	if (m_lastErrorMsg.size() >= 10)
		m_lastErrorMsg.pop();

	std::string rmsg = "";
	std::string rcode = "";
	// trans_u8 是翻译后的 字符串，可能是拼接的字符串，有值就用它
	TranslateBase(fmt_u8.c_str(), [&](const std::string& message, const std::string& code) {
		m_lastErrorMsg.push(std::make_pair(trans_u8==""? message: trans_u8, code));
	});
}

BBGLOBE::Log::Log()
	:m_language(EPLanguage::Chinese)
	//m_translateJson(nlohmann::json())
{
}

BBGLOBE::Log::~Log()
{
}

BBGLOBE::Log & BBGLOBE::Log::getInstance()
{
	static Log m_log;
	return m_log;
}

void BBGLOBE::Log::initLog(const std::string & logName, const std::string & fileName, EPType eptype, bool isMt /*= true*/, bool writeDebugToFile/* = false*/)
{
	std::lock_guard<std::mutex> lock(m_initMtx);
	std::string slogName = logName; // 控制台的log 名称为空
	auto itor = std::find_if(m_loglst.begin(), m_loglst.end(), [&](std::shared_ptr<spdlog::logger> log) {
		return log->name().compare(slogName) == 0;
	});
	// 不重复添加
	if (itor != m_loglst.end())
		return;
	if (fileName.empty())
	{
		//控制台显示debug日志，但不写入文件

		// release 不输出debug信息,加快速度
#ifndef NDEBUG
		spdlog::default_logger()->set_level(spdlog::level::debug);
#else
		spdlog::default_logger()->set_level(spdlog::level::info);
#endif
		m_loglst.insert(spdlog::default_logger());
	}
	else
	{
		std::shared_ptr<spdlog::logger> logger = nullptr;
		if (isMt)
		{
			switch (eptype)
			{
			case BBGLOBE::EPType::Daily:
				logger = spdlog::daily_logger_mt(slogName, fileName);
				break;
			case BBGLOBE::EPType::Base:
				logger = spdlog::basic_logger_mt(slogName, fileName);
				break;
			default:
				logger = spdlog::daily_logger_mt(slogName, fileName);
				break;
			}

		}
		else
		{
			switch (eptype)
			{
			case BBGLOBE::EPType::Daily:
				logger = spdlog::daily_logger_st(slogName, fileName);
				break;
			case BBGLOBE::EPType::Base:
				logger = spdlog::basic_logger_st(slogName, fileName);
				break;
			default:
				logger = spdlog::daily_logger_st(slogName, fileName);
				break;
			}
		}
		logger->set_level(writeDebugToFile? spdlog::level::debug : spdlog::level::info);
		m_loglst.insert(logger);
	}

	//if (m_translateJson == nlohmann::json())
	//{
	//	//1.读取GIM的错误信息配置文件
	//	std::string filePath = FileSystem::GetCurrentModuleDirectory() + "/jsons/language.json";
	//	if (!JsonUtil::ReadFile(filePath, m_translateJson))
	//		return ;

	//	//2.创建快速查询结构
	//	for (auto it = m_translateJson.begin(); it != m_translateJson.end(); ++it) {
	//		nlohmann::json obj = it.value();
	//		if (obj.find("en") != obj.end() && obj["en"].is_string()) {
	//			const std::string key = JsonUtil::GetString(obj,"en");
	//			m_fastTranslationHashMap[key] = &it.value();
	//		}
	//	}
	//}
}

void BBGLOBE::Log::removeLog(const std::string & logName)
{
	auto itor = std::find_if(m_loglst.begin(), m_loglst.end(), [&](std::shared_ptr<spdlog::logger> log) {
		return log->name().compare(logName) == 0;
	});
	if (itor != m_loglst.end())
	{
		(*itor)->flush();
		(*itor)->set_level(spdlog::level::off);
		m_loglst.erase(itor);
	}

	spdlog::drop(logName);
	spdlog::shutdown();
}


void BBGLOBE::Log::ExportLogInfo(const char * msg_u8, std::function<void(std::shared_ptr<spdlog::logger>, const std::string&)> customfunc)
{
	for (auto itor = m_loglst.begin(); itor != m_loglst.end(); ++itor)
	{
		auto log = *itor;
		if (!log)
			continue;

		if (log->name().empty())
			customfunc(log, StringConverter::GetTempUseString(msg_u8));
		else
			customfunc(log, msg_u8);

	}
}

std::string Tr(const std::string & msg)
{
	return StringConverter::GetTempUseString(LogIns.Translate(msg));
}

void BBGLOBE::Log::Assert(bool b)
{
	// b 为false，中断
	assert(b);
}