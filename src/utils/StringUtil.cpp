#include "StringUtil.h"
#include <stdarg.h>
#include <stdio.h>

using namespace BBGLOBE;

std::string StringUtil::Trimstr(const std::string& s)
{
	if (s.empty())
		return s;

	std::string str = s;
	str.erase(0, str.find_first_not_of(" "));
	str.erase(0, str.find_first_not_of("\t"));
	str.erase(0, str.find_first_not_of("\r"));
	size_t index = str.find_last_not_of(" ");
	if (index != std::string::npos )
		str.erase(index + 1);

	index = str.find_last_not_of("\r");
	if (index != std::string::npos)
		str.erase(index + 1);

	index = str.find_last_not_of("\t");
	if (index != std::string::npos)
		str.erase(index + 1);

	return str;
}

std::string StringUtil::DeleteSpecialStr(const std::string& s)
{
	std::string str = s;
	std::regex pattern("[#=+%& ?]");
	return regex_replace(str, pattern, "");
}

int StringUtil::Compare(const char* str1, const char* str2, bool bIgnoreCase)
{
	if (!bIgnoreCase)
	{
		return strcmp(str1, str2);
	}
	else
	{
#ifdef _WIN32
		return _stricmp(str1, str2);
#else
		return strcasecmp(str1, str2);
#endif
	}
}

bool StringUtil::IsEqual(const std::string & str1, const std::string & str2, bool bIgnoreCase)
{
	return IsEqual(str1.c_str(),str2.c_str(),bIgnoreCase);
}

bool StringUtil::IsEqual(const char* str1, const char* str2, bool bIgnoreCase)
{
	return StringUtil::Compare(str1, str2, bIgnoreCase) == 0;
}

bool StringUtil::IsNullOrEmpty(const char* str)
{
	return str == nullptr || std::strlen(str) == 0;
}

void StringUtil::StringUpperOrLowerCast(std::string& str, bool toLower)
{
	if (toLower) {
		std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	}
	else {
		std::transform(str.begin(), str.end(), str.begin(), ::toupper);
	}
}

std::string& StringUtil::ReplaceAllString(std::string& str, const std::string& old_value, const std::string& new_value)
{
	for (std::string::size_type pos(0); pos!= std::string::npos; pos+=new_value.length())
	{
		if ((pos = str.find(old_value, pos)) != std::string::npos)
			str.replace(pos, old_value.length(), new_value);
		else
			break;
	}

	return  str;
}

std::vector<std::string> StringUtil::Split(const std::string& str, std::string pattern, bool withEndEmpty)
{
	std::string copyStr = str;
	std::string::size_type pos;
	std::vector<std::string> result;
	copyStr += pattern;//扩展字符串以方便操作
	size_t size = copyStr.size();

	for (size_t i = 0; i < size; i++)
	{
		pos = copyStr.find(pattern, i);
		if (pos < size)
		{
			std::string s = copyStr.substr(i, pos - i);
			result.push_back(s);
			i = pos + pattern.size() - 1;
		}
	}

	size_t resultSize = result.size();
	if (!withEndEmpty &&resultSize >0 && result[resultSize-1] == "")
	{
		// 移除最后一个元素
		result.pop_back();
	}

	return result;
}

bool StringUtil::IsNumber(std::string str)
{
	for (size_t i = 0; i < str.size(); i++) {
		if (str.at(i) == '-' && str.size() > 1)
			continue;
		if (str.at(i) > '9' || str.at(i) < '0')
			return false;
	}
	return true;
}

double StringUtil::SafeStod(const std::string& argument, double defaultValue )
{
	if (argument.empty())
		return 0.0;
	std::string exception;
	return StringUtil::SafeStod(argument, exception,defaultValue);
}

double StringUtil::SafeStod(const std::string& argument, std::string& exception, double defaultValue )
{
	try {
		std::size_t pos;
		double resultNumber = std::stod(argument, &pos);
		if (pos < argument.size())
		{
			exception = "Invalid_argument";
			return defaultValue;
		}
		return resultNumber;
	}
	catch (std::invalid_argument&) {
		exception = "Invalid_argument";
	}
	catch (std::out_of_range&) {
		exception = "Out of range";
	}
	catch (...) {
		exception = "Something else";
	}
	return defaultValue;
}

int StringUtil::SafeStoi(const std::string& argument)
{
	if (argument.empty())
		return 0;
	std::string exception;
	return SafeStoi(argument, exception);
}

int StringUtil::SafeStoi(const std::string& argument, std::string& exception)
{
	try {
		if (!StringUtil::IsNumber(argument)) {
			exception = "Is not an integer";
			return -1;
		}
		return std::stoi(argument);
	}
	catch (std::invalid_argument&) {
		exception = "Invalid_argument";
	}
	catch (std::out_of_range&) {
		exception = "Out of range";
	}
	catch (...) {
		exception = "Something else";
	}
	return 0;
}

std::string StringUtil::ConvertStringToVaildFileName(const std::string& str)
{
	std::string result;
	for (const auto c : str) {
		if (c == '\\' || c == '/' || c == ':' || c == '*' || c == '?' || c == '"' || c == '<' || c == '>' || c == '|') {
			result.push_back(' ');
		}
		else {
			result.push_back(c);
		}
	}
	return result;
}

std::string StringUtil::GetServerSafedFileName(const std::string& originalFileName, const std::string& alterString)
{
	std::string tmpOriginalFileName = originalFileName;
	std::vector<std::string> forbiddenStrs = {
		"!","@","#","$","%","&","*","(",")","=","+","-","{","}","\\",";",":","\"",",",".","/","<",">","?","'"," ","and","or"
	};
	for (auto& str : forbiddenStrs) {
		ReplaceAllString(tmpOriginalFileName, str, alterString);
	}
	return tmpOriginalFileName;
}

// 获得格式化的字符串
std::string StringUtil::GetFormattedString(const char* format, ...) {
	char buf[4096];
	va_list args;
	va_start(args, format);
	vsnprintf(buf, 4096, format, args);
	va_end(args);
	return std::string(buf);
}

bool StringUtil::StartWith(const std::string & str, const std::string & content, bool bIgnoreCase)
{
	if (str.size() < content.size())
		return false;
	std::string subStr = str.substr(0, content.size());
	return IsEqual(subStr, content, bIgnoreCase);
}

bool StringUtil::EndWith(const std::string & str, const std::string & content, bool bIgnoreCase)
{
	if (str.size() < content.size())
		return false;
	std::string subStr = str.substr(str.size() - content.size(), content.size());
	return IsEqual(subStr, content, bIgnoreCase);
}

std::string StringUtil::ToLower(const std::string & str)
{
	std::string res = str;
	std::transform(str.begin(), str.end(), res.begin(), ::tolower);
	return res;
}

std::string StringUtil::ToUpper(const std::string & str)
{
	std::string res = str;
	std::transform(str.begin(), str.end(), res.begin(), ::toupper);
	return res;
}

bool StringUtil::IsValidNum(std::string numString)
{
	if (numString.empty())
		return false;
	if (numString[0] == '-')
		numString = numString.substr(1);

	if (isdigit(*(numString.c_str())))
	{
		transform(numString.begin(), numString.end(), numString.begin(), ::tolower);
		return numString != "1.#qnan0" && numString != "1.#ind00";
	}
	else
	{
		return false;
	}
}

glm::dmat4 StringUtil::StringToMatrix(const std::string& strMatrix)
{
	glm::dmat4 tempMatrix(1.0);
	std::vector<std::string> vecMatrix = Split(strMatrix, ",");
	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			tempMatrix[i][j] = stod(vecMatrix[4 * i + j]);
		}
	}
	return tempMatrix;
}

std::string StringUtil::MatrixToString(const glm::dmat4& matrix)
{
	glm::dmat4 tempMatrix(1.0);
	std::stringstream ss;
	for (size_t i = 0; i < 4; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			if (i == 3 && j == 3)
				ss << matrix[i][j];
			else
				ss << matrix[i][j] << ",";
		}
	}
	return ss.str();
}

std::string StringUtil::Join(const std::vector<std::string>& vec,const std::string& str)
{
	std::string result;
	for (size_t i = 0; i < vec.size(); i++)
	{
		result.append(vec[i]);
		if (i != vec.size() - 1)
			result.append(str);
	}
	return result;
}

std::string StringUtil::RandomStrGen(int length)
{
	std::string charset = "abcdefghijklmnopqrstuvwxyz1234567890";
	std::string result;
	result.resize(length);

	srand(time(NULL));
	for (int i = 0; i < length; i++)
	{
		result[i] = charset[rand() % charset.length()];
	}
	return result;
}