/**
*  Copyright (c) 2019, 亿力吉奥
*  All rights reserved.
*
*  @file        StringConverter.h
*  @author      huqi
*  @date        2020-08
*
*  @brief		编码转换
*  @note        无
*  @version
*/
#pragma once

#include <string>
#include <codecvt>

namespace BBGLOBE {

	/**
	* \brief	编码转换
	* \author	huqi
	* \date		2020-08
	*/
	class StringConverter
	{
	public:
		/**
		* \brief Unicode转UTF8
		*/
		static std::string UnicodeToUTF8(const std::wstring & wstr);
		/**
		* \brief UTF8转Unicode
		*/
		static std::wstring UTF8ToUnicode(const std::string & str);
		/**
		* \brief Unicode转ANSI
		*/
		static std::string UnicodeToANSI(const std::wstring & wstr);
		/**
		* \brief ANSI转Unicode
		*/
		static std::wstring ANSIToUnicode(const std::string & str);
		/**
		* \brief String转Binary
		*/
		static std::string StringToBinary(const std::string& str);
		/**
		* \brief Binary转String
		*/
		static std::string BinaryToString(const std::string& bin);
		/**
		* @brief 外部进入的字符串一律为utf-8，在win上转换为临时变量 ansi(否则判断路径错误)
		*/
		static std::string GetTempUseString(const std::string& inStr);
		/**
		* @brief 保存和内部程序变量流通的字符串一律为utf-8
		*/
		static std::string GetTempU8String(const std::string& inStr);
	private:
		/**
		* \brief UTF8转ANSI
		*/
		static std::string UTF8ToANSI(const std::string & str);
		/**
		* \brief ANSI转UTF8
		*/
		static std::string ANSIToUTF8(const std::string & str);

	};
}// end of namespace EPGIS_3D
