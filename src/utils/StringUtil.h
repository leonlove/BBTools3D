/*****************************************************************//**
 * @file   String.h
 * @brief  字符串处理帮助类
 *
 * @author zhangqiankun
 * @date   August 2020
 *********************************************************************/

#pragma once

#include "stdafx.h"
#include "glm/ext/matrix_double4x4.hpp"

namespace BBGLOBE {

class StringUtil {

public:
	/**
	 * @brief 去除首尾空白.
	 *
	 * @param s 输入字符串
	 * @return 返回去除空白后的字符串
	 */
	static std::string Trimstr(const std::string& s);

	/**
	* @brief 使用正则表达式过滤特殊字符.
	*
	* @param s 输入字符串
	* @return 返回去除特殊字符后的字符串
	*/
	static std::string DeleteSpecialStr(const std::string& s);
	/**
	 * @brief 比较两个字符串是否相等
	 * @return 相等返回0 str1>str2返回值大于0，str1<str2返回值小于0
	 */
	static int Compare(const char* str1, const char* str2, bool bIgnoreCase = true);

	/**
	 * @brief 比较两个字符串是否相等
	 */

	static bool IsEqual(const std::string& str1, const std::string& str2, bool bIgnoreCase = true);
	static bool IsEqual(const char* str1, const char* str2, bool bIgnoreCase = true);

	/**
	 * @brief 判断字符串是否为空
	 */
	static bool IsNullOrEmpty(const char* str);

	/**
	 * @brief 字符串大小写转换
	 */
	static void StringUpperOrLowerCast(std::string& str, bool toLower);

	/**
	 * @brief 替换字符串.
	 *
	 * @param str 输入字符串
	 * @param old_value 待替换字符串
	 * @param new_value 替换后字符串
	 * @return 替换后的字符串
	 */
	static std::string& ReplaceAllString(std::string& str, const std::string& old_value, const std::string& new_value);

	/**
	 * @brief 分割字符串.
	 *
	 * @param str 输入字符串
	 * @param pattern 分割字符
	 * @param pattern 是否保留数组最后的空元素
	 * @return 返回分割后字符串数组
	 */
	static std::vector<std::string> Split(const std::string& str, std::string pattern,bool withEndEmpty = true);

	/**
	 * @brief 是否是数字.
	 */
	static bool IsNumber(std::string str);

	/**
	 * @brief 字符串转double.
	 */
	static double SafeStod(const std::string& argument,double defaultValue=0.0);

	/**
	 * @brief 字符串转double.
	 */
	static double SafeStod(const std::string& argument, std::string& exception, double defaultValue = 0.0);

	/**
	 * @brief 字符串转int.
	 */
	static int SafeStoi(const std::string& argument);

	/**
	 * @brief 字符串转int.
	 */
	static int SafeStoi(const std::string& argument, std::string& exception);

	/**
	* @brief 字符串显示精度.
	* @param n 保留小数位数
	*/
	template <typename T>
	static inline std::string to_string_with_precision(const T a_value, const int n = 6)
	{
		std::locale::global(std::locale(std::locale::classic(), "", std::locale::ctype));
		std::ostringstream out;
		out.setf(std::ios::fixed);  // 保留两位小数
		out << std::setprecision(n) << a_value;
		return out.str();
	}

	/**
	* @brief 将字符串转换成可以正常储存的文件名
	*/
	static std::string ConvertStringToVaildFileName(const std::string& str);

	/**
	* @brief 获得服务器安全的储存文件名
	*/
	static std::string GetServerSafedFileName(const std::string& originalFileName, const std::string& alterString);

	/**
	* @brief 获得格式化后的字符串
	*/
	static std::string GetFormattedString(const char* format, ...);
	/**
	* @brief 以字符串开头.
	*/
	static bool StartWith(const std::string & str, const std::string & content, bool bIgnoreCase = true);
	/**
	* @brief 以字符串结尾.
	*/
	static bool EndWith(const std::string & str, const std::string & content, bool bIgnoreCase = true);
	/**
	* @brief 字符串转为小写.
	*/
	static std::string ToLower(const std::string& str);
	/**
	* @brief 字符串转为大写.
	*/
	static std::string ToUpper(const std::string& str);
	/**
	* @brief 字符串是否为有效的数字.
	*/
	static bool IsValidNum(std::string numString);
	/**
	* @brief 字符串转矩阵.
	*/
	static glm::dmat4 StringToMatrix(const std::string& strMatrix);
	/**
	* @brief 矩阵转字符串.
	*/
	static std::string MatrixToString(const glm::dmat4& matrix);
	/**
	* @brief 字符串连接.
	*/
	static std::string Join(const std::vector<std::string>& vec, const std::string& str);
	/**
	* @brief 随机生成字符串.
	*/
	static std::string RandomStrGen(int length);

	static bool ContainsChineseCharacters(const std::string& str) {
		// 使用正则表达式匹配中文字符
		std::regex chineseRegex(u8"[\u4e00-\u9fa5]+");

		// 在字符串中搜索匹配的中文字符
		return std::regex_search(str, chineseRegex);
	}

};

#define Fmt StringUtil::GetFormattedString                                                                                                           
}
