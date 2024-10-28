#include "StringConverter.h"
#include <memory>
#include <iostream>
#include <sstream>
#include <fstream>
#include <codecvt>
#include <locale>
#include <memory>
#include <string.h>

using namespace std;
using namespace BBGLOBE;

std::string StringConverter::UnicodeToUTF8(const std::wstring& wstr)
{
	//setlocale(LC_CTYPE, "");
	std::string ret;
	try {
		std::wstring_convert< std::codecvt_utf8<wchar_t> > wcv;
		ret = wcv.to_bytes(wstr);
	}
	catch (const std::exception& e) {
		throw e;
	}
	return ret;
}

std::wstring StringConverter::UTF8ToUnicode(const std::string& str)
{
	//setlocale(LC_CTYPE, "");
	std::wstring ret;
	try {
		std::wstring_convert< std::codecvt_utf8<wchar_t> > wcv;
		ret = wcv.from_bytes(str);
	}
	catch (const std::exception& e) {
		throw e;
	}
	return ret;
}

std::string StringConverter::UnicodeToANSI(const std::wstring& wstr)
{
	//setlocale(LC_CTYPE, "");
	std::string ret;
	std::mbstate_t state = {};
	const wchar_t* src = wstr.data();
	size_t len = std::wcsrtombs(nullptr, &src, 0, &state);
	if (static_cast<size_t>(-1) != len) {
		std::unique_ptr< char[] > buff(new char[len + 1]);
		len = std::wcsrtombs(buff.get(), &src, len, &state);
		if (static_cast<size_t>(-1) != len) {
			ret.assign(buff.get(), len);
		}
	}
	return ret;
}

std::wstring StringConverter::ANSIToUnicode(const std::string& str)
{
	//setlocale(LC_CTYPE, "");
	std::wstring ret;
	std::mbstate_t state = {};
	const char* src = str.data();
	size_t len = std::mbsrtowcs(nullptr, &src, 0, &state);
	if (static_cast<size_t>(-1) != len) {
		std::unique_ptr< wchar_t[] > buff(new wchar_t[len + 1]);
		len = std::mbsrtowcs(buff.get(), &src, len, &state);
		if (static_cast<size_t>(-1) != len) {
			ret.assign(buff.get(), len);
		}
	}
	return ret;
}

std::string StringConverter::UTF8ToANSI(const std::string& str)
{
	try {
		return UnicodeToANSI(UTF8ToUnicode(str));
	}
	catch (const std::exception&) {
		return str;
	}
}

std::string StringConverter::ANSIToUTF8(const std::string& str)
{
	//setlocale(LC_CTYPE, "");
	try {
		return UnicodeToUTF8(ANSIToUnicode(str));
	}
	catch (const std::exception&) {
		return str;
	}
}

std::string StringConverter::StringToBinary(const std::string& str)
{
	std::string ret;
	char s[100];
	for (auto c : str) {
		snprintf(s, sizeof(s), "%02X", (unsigned char)(c));
		ret += s;
	}
	return ret;
}

std::string StringConverter::BinaryToString(const std::string& bin)
{
	std::string ret;
	for (size_t i = 0; i < bin.size(); i += 2) {
		std::string str;
		str.push_back(bin[i]);
		str.push_back(bin[i + 1]);
		int c = 0;
		sscanf(str.c_str(), "%02X", &c);
		ret.push_back((char)c);
	}
	return ret;
}

/*外部进入的字符串一律为utf-8，在win上转换为临时变量 ansi(否则判断路径错误)*/
std::string StringConverter::GetTempUseString(const std::string & inStr)
{
#ifdef _WIN32
	return StringConverter::UTF8ToANSI(inStr);
#else
	return inStr;
#endif // _WIN32
}

/*保存和内部程序变量流通的字符串一律为utf-8*/
std::string StringConverter::GetTempU8String(const std::string & inStr)
{
#ifdef _WIN32
	return StringConverter::ANSIToUTF8(inStr);
#else
	return inStr;
#endif // _WIN32
}

