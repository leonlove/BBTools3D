/*********************************************************************
 * @file   FileSystemUtil.h
 * @brief  文件系统
 *
 * @author wanya
 * @date   August 2024
 *********************************************************************/

#pragma once

#include "stdafx.h"

namespace BBGLOBE
{
	enum EPFileEncoding
	{
		FE_ANSI,
		FE_UNICODE,
		FE_UNICODE_BIG_ENDIAN,
		FE_UTF8,
		FE_UTF8_BOM,
		FE_NULL
	};

	// 压缩方式
	enum CompressOptions
	{
		CO_NONE = 0,	//明文无压缩
		CO_GZIP = 1,
		CO_DEFLATE = 2,
		CO_BR = 3
	};

	class FileSystem
	{
	public:
		/**
		 * @brief 路径是否存在.
		 *
		 * @param path 路径
		 * @return 不存在返回0，文件夹返回1,文件返回-1
		 */
		static int IsFileOrDir(const std::string& path);

		/**
		 * @brief 文件是否存在.
		 *
		 * @param fileName 文件路径
		 * @return 存在返回ture,不存在返回false
		 */
		static bool FileExists(const std::string& fileName);

		/**
		 * @brief 文件夹是否存在.
		 *
		 * @param fileName 文件夹路径
		 * @return 存在返回ture,不存在返回false
		 */
		static bool DirExists(const std::string& dirName_in);
		static bool U8DirExists(const std::string& dirName_in);


		/**
		* @brief 文件夹是否存在,如果不存在，则创建新的文件夹.
		*
		* @param fileName 文件夹路径
		* @return 存在返回ture,不存在返回false
		*/
		static bool CheckDirExistsOrCreate(const std::string& dirName_in);

		/**
		 * @brief 文件夹是否为空.
		 *
		 * @param dirname 文件夹路径
		 * @return 如果为空返回ture,否则返回false
		 */
		static bool IsDirEmpty(const std::string& dirname);

		/**
		 * @brief 创建文件夹.
		 *
		 * @param strSrcFilePath 文件夹路径
		 * @return 是否创建成功
		 */
		static bool CreateDir(const std::string& strSrcFilePath);
		static bool CreateU8Dir(const std::string& strSrcFilePath);

		/**
		* @brief 创建系统缓存文件夹.
		*
		* @param dirPath 文件夹路径
		* @return 是否创建成功
		*/
		static bool CreateSysTemDir(std::string& dirPath);

		/**
		 * @brief 清除文件夹.
		 *
		 * @param szFileDir 文件架路径
		 * @param removeInputDir 是否删除文件夹
		 * @return 返回是否成功
		 */
		static bool ClearDir(const std::string& szFileDir, bool removeInputDir = true);
		static bool ClearDir(const char* szFileDir, bool removeInputDir = true);
		static bool ClearU8Dir(const char* szFileDir, bool removeInputDir = true);

		/**
		 * @brief 删除文件.
		 *
		 * @param fileName 文件路径
		 * @return 返回是否删除成功
		 */
		static bool FileDelete(const std::string& fileName, bool showLog = true);

		/**
		 * @brief 文件夹重命名.
		 *
		 * @param srcFileName 原始文件路径
		 * @param dstFileName 目标文件夹路径
		 * @return 返回是否成功
		 */
		static bool FileRename(const std::string& srcFileName, const std::string& dstFileName);

		/**
		 * @brief 递归获取文件夹下所有文件.
		 *
		 * @param cate_dir 输入文件夹
		 * @param files 输出文件名称数组
		 * @param format 过滤格式字符串,使用“*”或者指定文件后缀
		 * @param recursive 是否递归
		 */
		static void GetFiles(std::string cate_dir, std::vector<std::string>& files, const std::string& format, bool recursive = true);

		/**
		 * @brief 获取当前目录下的所有子目录.
		 *
		 * @param cate_dir 输入文件架目录
		 * @param dirs 返回子目录路径数组
		 */
		static void GetSubDirs(std::string cate_dir, std::vector<std::string>& dirs, bool recursive = false);

		/**
		 * @brief 提取文件名称.
		 *
		 * @param fullPathFileName 输入文件路径
		 * @param hasExt 是否包含文件扩展名称
		 * @return 返回文件名称
		 */
		static std::string ExtractFileName(const std::string& fullPathFileName, bool hasExt = true);

		/**
		 * @brief 去掉文件扩展名.
		 *
		 * @param fullPathFileName 输入文件名称或文件路径
		 * @return 返回去掉扩展名的字符串
		 */
		static std::string EraseExtName(const std::string& fullPathFileName);

		/**
		 * @brief 获取文件路径扩展名，不包含".".
		 *
		 * @param fullPathFileName 文件名称或文件路径
		 * @return 文件扩展名称
		 */
		static std::string GetExtName(const std::string& fullPathFileName);

		/**
		 * @brief 获取本地操作系统风格的路径分隔复.
		 */
		static std::string PathSeparator();

		/**
		 * @brief 获取本地操作系统风格的路径
		 */
		static std::string NativePath(const std::string& path);

		/**
		 * @brief 获取文件夹名称
		 */
		static std::string GetPathName(const std::string& path);

		/**
		 * @brief 合并路径
		 */
		static std::string CombinePath(const std::string& path1, const std::string& path2);

		/**
		 * @brief 计算相对路径
		 */
		static std::string RelativePath(const std::string& from, const std::string& to);

		/**
		 * @brief 获取父级路径
		 */
		static std::string ParentPath(const std::string& path);

		/**
		 * @brief 获取当前目录.
		 */
		static const std::string GetCurrentDir();

		/**
		 * @brief 复制文件.
		 *
		 * @param srcFile 源文件
		 * @param destFile 目标文件,如果目标文件不存在则创建，如果存在则覆盖
		 * @return 返回是否复制成功
		 */
		static bool FileCopy(const std::string& srcFile, const std::string& destFile);
		static bool U8FileCopy(const std::string& srcFile, const std::string& destFile);

		/**
		 * @brief 复制目录树
		 * @param sourcePath 源路径
		 * @param destPath 目标路径
		 * @return 返回是否复制成功
		*/
		static bool TreeCopy(const std::string& sourcePath, const std::string& destPath);

		/**
		 * @brief 写文件.
		 *
		 * @param filename 文件路径
		 * @param buf 待写入的buffer
		 * @param buf_len buffer的长度
		 * @return 返回是否写入成功
		 */
		static bool WriteFile(const char* filename, const char* buf, unsigned long buf_len);
		static bool WriteU8File(const std::string& fileName, const std::string& content);

		static bool SaveFile(const std::string& keyOrFileName, const std::string& content, CompressOptions co);
		static bool ReadFile(const std::string& keyOrFileName, std::string& content);

		static bool ReadFile(const char* filename, char** buf,int& len);
		static bool ReadU8File(const std::string& fileName, std::string& content);
		static bool ReadFileString(const std::string& fileName, std::string& content);

		static bool IsFileEmpty(const std::string& filename);
		/**
		 * @brief 获取当前动态库的路径.
		 */
		static std::string GetCurrentModuleDirectory();

		static bool GetFileSize(const std::string& filepath, double& fileSize, std::string& fileSizeUnit);
		/**
		 * @brief 判断是否是绝对路径.
		 * 
		 * @param path
		 * @return 
		 */
		static bool IsAbsolutePath(const std::string& path);


		/**
		 * 修改文件名称，替换非法字符.
		 * 非法字符包括 ! @ # $ % & * ( ) = + - { } \ ; : “ , . / < > ? ‘ 空格 and or
		 * @param fileName
		 * @return 
		 */
		static bool AdjustFileName(std::string& fileName, const std::string& replaceStr = "_");

		static bool AdjustFileNameReName(std::string& fileName);

		/**
		* 是否是有效的文件名
		* 非法字符包括  '!','@','#','$','%','&','*','(',')','=','+','-','{','}','\\',';',':','\"',',','/','<','>','?','\'' 空格 and or
		*/
		static bool IsVaildFileName(const std::string& fileName);
		/**
		* \brief		读取文件内容，识别文件编码
		*/
		static EPFileEncoding ReadFileEncoding(const std::string & filePath);
		static EPFileEncoding ReadU8FileEncoding(const std::string & filePath);

		static void GetU8Files(const std::string& cate_dir, std::vector<std::string>& files, const std::string& format, bool recursive = true);

		static void GetU8SubDirs(std::string cate_dir, std::vector<std::string>& dirs, bool recursive = false);

		static bool CheckU8DirExistsOrCreate(const std::string& dir);
		static bool U8FileExists(const std::string & fileName);

		static unsigned long long GetFileSize(const std::string& filePath_use);
		/**
		* \brief		移除路径末尾的分隔符
		*/
		static std::string RemoveEndSeparator(const std::string& pathin);
	private:
		//无法百分百判断文件是 无bom utf-8 还是 ansi  这几个函数设为私有
		/**
		* \brief 是否为UTF8内容
		*/
		static bool IsUTF8Text(const void* pBuffer, long size);
		/**
		* \brief 是否为UTF8内容
		*/
		static bool IsUTF8Text(const std::string& str);

		static bool CheckUtf8WithoutBom(const std::string& content);
	};
}
