#include "FileSystemUtil.h"
#include "StringConverter.h"
#include "StringUtil.h"
#include "Types.h"
#include "Log.h"

#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#include <io.h>
#include <fileapi.h>
#include <filesystem>
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#endif
//#include "Gzip.h"

#ifndef MAX_PATH
#define MAX_PATH 512
#endif

using namespace BBGLOBE;

int FileSystem::IsFileOrDir(const std::string& path)
{
#ifdef WIN32
	DWORD dwAttr = GetFileAttributes((LPCSTR)path.c_str());
	//Not exist
	if (dwAttr == 0xFFFFFFFF)
		return 0;
	//exist
	else if (dwAttr & FILE_ATTRIBUTE_DIRECTORY)
		return 1;
	//is File
	else
		return -1;
#else
	if (0 == access(path.c_str(), 0))
	{
		struct stat* buf;

		buf = (struct stat*) malloc(sizeof(struct stat));
		memset(buf, 0, sizeof(struct stat));

		stat(path.c_str(), buf);

		if (S_ISDIR(buf->st_mode))
		{
			free(buf);
			buf = NULL;
			return 1;
		}
		else
		{
			free(buf);
			buf = NULL;
			return -1;
		}
	}

	return 0;

#endif
}

bool FileSystem::FileExists(const std::string& fileName)
{
	return -1 == IsFileOrDir(fileName);
}

bool FileSystem::DirExists(const std::string& dirName_in)
{
	return 1 == IsFileOrDir(dirName_in);
}

bool FileSystem::U8DirExists(const std::string & dirName_in)
{
	std::string dirLocal = StringConverter::GetTempUseString(dirName_in);
	return DirExists(dirLocal);
}

bool FileSystem::CheckDirExistsOrCreate(const std::string & dirName_in)
{
	if (DirExists(dirName_in))
		return true;
	return CreateDir(dirName_in);
}

bool FileSystem::IsDirEmpty(const std::string& dirname)
{
	bool isEmpty = true;
#ifdef WIN32
	_finddata_t file;
	long long lf = 0;
	std::string filename = dirname + osDirSep + "*";
	if ((lf = _findfirst(filename.c_str(), &file)) == -1)
		return true;
	else {
		std::string  curpath = dirname;
		do
		{
			if (strcmp(file.name, ".") == 0 || strcmp(file.name, "..") == 0)
				continue;
			isEmpty = false;
			break;
		} while (_findnext(lf, &file) == 0);
		_findclose(lf);
	}
#else
	DIR* dir;
	struct dirent* ptr;

	if ((dir = opendir(dirname.c_str())) == NULL)
		return true;
	while ((ptr = readdir(dir)) != NULL)
	{
		if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0)    ///current dir OR parrent dir
			continue;
		isEmpty = false;
		break;
	}
	closedir(dir);
#endif
	return isEmpty;
}

bool FileSystem::CreateDir(const std::string& strSrcFilePath)
{
	if (FileSystem::DirExists(strSrcFilePath) ) {
		return true;
	}

	std::string strFilePath = strSrcFilePath;
	StringUtil::ReplaceAllString(strFilePath, "\\", "/");

	//不管文件夹路径中是否带 . 都应该能创建
	strFilePath += osDirSep;

	size_t startPos = 0;
	size_t endPos;

	while (true)
	{
		endPos = strFilePath.find(osDirSep, startPos);
		if (endPos != std::string::npos)
		{
			std::string strFolderPath = strFilePath.substr(0, endPos);
			startPos = endPos + std::string::size_type(1);

			if (strFolderPath.rfind(":") == (strFolderPath.size() - 1))
			{
				continue;
			}

			struct stat fileStat = { 0 };
			if (stat(strFolderPath.c_str(), &fileStat) == 0)
			{
				if (!(fileStat.st_mode & S_IFDIR))
				{
					return false;
				}
			}
			else
			{
#ifdef WIN32
				int flag = mkdir(strFolderPath.c_str());
#else
				int flag = mkdir(strFolderPath.c_str(), 0777);
#endif
				if (-1 == flag)
				{
					return false;
				}
			}

			continue;
		}

		break;
	}
	return true;
}

bool FileSystem::CreateU8Dir(const std::string & strSrcFilePath)
{
	std::string dirLocal = StringConverter::GetTempUseString(strSrcFilePath);
	return CreateDir(dirLocal);
}

bool FileSystem::CreateSysTemDir(std::string & dirPath)
{
#ifdef WIN32
	dirPath = std::string(std::tmpnam(nullptr));
#else
	char temp[] = "/tmp/tempfileXXXXXX";
	dirPath = std::string(mkdtemp(temp));
#endif
	return FileSystem::CreateDir(dirPath);
}

bool FileSystem::ClearDir(const std::string & szFileDir, bool removeInputDir)
{
	if (szFileDir.empty())
		return true;
	return ClearDir(szFileDir.c_str(),removeInputDir);
}

bool FileSystem::ClearDir(const char* szFileDir, bool removeInputDir)
{
	if (0 == IsFileOrDir(szFileDir)) {
		//LogIns.debug("File or Dir not Exists{}", szFileDir);
		return true;
	}
#ifdef WIN32
	std::string strDir = szFileDir;
	if (strDir.at(strDir.length() - 1) != '\\' || strDir.at(strDir.length() - 1) != '/') {
		strDir += "\\";
	}
	WIN32_FIND_DATA wfd;
	HANDLE hFind = FindFirstFile((strDir + "*.*").c_str(), &wfd);
	if (hFind == INVALID_HANDLE_VALUE)
		return false;
	do
	{
		if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (stricmp(wfd.cFileName, ".") != 0 &&
				stricmp(wfd.cFileName, "..") != 0)
				FileSystem::ClearDir((strDir + wfd.cFileName).c_str());
		}
		else
		{
			SetFileAttributes((strDir + wfd.cFileName).c_str(),FILE_ATTRIBUTE_NORMAL);
			DeleteFile((strDir + wfd.cFileName).c_str());
		}
	} while (FindNextFile(hFind, &wfd));
	FindClose(hFind);
	if (removeInputDir)
		return RemoveDirectory(szFileDir);

	return true;
#else
	char cur_dir[] = ".";
	char up_dir[] = "..";
	char dir_name[512];
	DIR* dirp;
	struct dirent* dp;
	struct stat dir_stat;

	if (0 != access(szFileDir, F_OK)) {
		return false;
	}

	if (0 > stat(szFileDir, &dir_stat)) {
		perror("get directory stat error");
		return false;
	}

	if (S_ISREG(dir_stat.st_mode)) {
		remove(szFileDir);
	}
	else if (S_ISDIR(dir_stat.st_mode)) {
		dirp = opendir(szFileDir);
		while ((dp = readdir(dirp)) != NULL) {
			if ((0 == strcmp(cur_dir, dp->d_name)) || (0 == strcmp(up_dir, dp->d_name))) {
				continue;
			}

			sprintf(dir_name, "%s/%s", szFileDir, dp->d_name);
			FileSystem::ClearDir(dir_name);
		}
		closedir(dirp);

		if (removeInputDir)
			return rmdir(szFileDir)==0; //成功返回0 否则-1
	}
	else {
		perror("unknow file type!");
		return false;
	}
	return true;
#endif
}

bool FileSystem::ClearU8Dir(const char * szFileDir, bool removeInputDir)
{
	std::string dirLocal = StringConverter::GetTempUseString(szFileDir);
	return ClearDir(dirLocal, removeInputDir);
}

bool FileSystem::FileDelete(const std::string& fileName, bool showLog)
{
	int flag = remove(fileName.c_str());

	if (flag != 0 && showLog ) {
		LogIns.error("{0} deleted failed ! Flag={1}", StringConverter::GetTempU8String(fileName.c_str()), flag);
	}

	return (flag == 0);
}

bool FileSystem::FileRename(const std::string& srcFileName, const std::string& dstFileName)
{
	int flag = rename(srcFileName.c_str(), dstFileName.c_str());
	if (flag != 0) {
		LogIns.error("Error: rename fail, {0}", strerror(errno));
		//失败后尝试重复执行100次，确保设备能响应
		for (int i = 0; i < 100; i++) {
			flag = rename(srcFileName.c_str(), dstFileName.c_str());
			if (flag == 0) {
				break;
			}
		}
	}

	return (flag == 0);
}

void FileSystem::GetFiles(std::string cate_dir, std::vector<std::string>& files, const std::string& format, bool recursive)
{
	if (cate_dir.empty())
	{
		return;
	}
#ifdef WIN32
	_finddata_t file;
	long long lf = 0;
	std::string filename = cate_dir + osDirSep + "*";
	if ((lf = _findfirst(filename.c_str(), &file)) == -1) {
		LogIns.error("{0} not found!", StringConverter::GetTempU8String(filename));
	}
	else {
		std::string  curpath = cate_dir;
		do
		{
			if (strcmp(file.name, ".") == 0 || strcmp(file.name, "..") == 0)
				continue;
			if ((file.attrib & _A_SUBDIR))
			{
				std::string otherpath = cate_dir + osDirSep + file.name;
				if (recursive)
					FileSystem::GetFiles(otherpath, files, format);
			}
			else {
				std::string filename1 = file.name;
				std::string ext = filename1.substr(filename1.find_last_of(".") + 1);
				transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
				if (ext == format || format == "*") {
					std::string filePath = FileSystem::CombinePath(curpath,file.name);
					//filePath = StringConverter::ANSIToUTF8(filePath);
					files.push_back(filePath);
				}
			}
		} while (_findnext(lf, &file) == 0);
		_findclose(lf);
	}
#else
	DIR* dir;
	struct dirent* ptr;

	if ((dir = opendir(cate_dir.c_str())) == NULL)
	{
		LogIns.error("{0} not found!", cate_dir);
		exit(1);
	}

	while ((ptr = readdir(dir)) != NULL)
	{
		if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0)    ///current dir OR parrent dir
			continue;
			
		std::string filename = ptr->d_name;
		std::string otherpath = cate_dir + "/" + ptr->d_name;
		if (DirExists(otherpath))
		{
			if (recursive)
				FileSystem::GetFiles(otherpath, files, format);
		}
		else
		{
			if (format != "*")
			{
				std::string ext = filename.substr(filename.find_last_of(".") + 1);
				transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
				if (ext == format) {
					files.push_back(cate_dir + "/" + filename);
				}
			}
			else
			{
				files.push_back(cate_dir + "/" + filename);
			}
		}
	}
	closedir(dir);
#endif
}
void FileSystem::GetSubDirs(std::string cate_dir, std::vector<std::string>& dirs, bool recursive)
{
#ifdef WIN32
	_finddata_t file;
	long long lf = 0;
	std::string filename = cate_dir + PathSeparator() + "*";
	if ((lf = _findfirst(filename.c_str(), &file)) == -1) {
		LogIns.error("{0} not found!", StringConverter::GetTempU8String(cate_dir));
	}
	else {
		std::string  curpath = cate_dir;
		do
		{
			if (strcmp(file.name, ".") == 0 || strcmp(file.name, "..") == 0)
				continue;
			if ((file.attrib & _A_SUBDIR))
			{
				std::string otherpath = cate_dir + PathSeparator() + file.name;
				if (recursive)
					FileSystem::GetSubDirs(otherpath, dirs, recursive);
				dirs.push_back(otherpath);
			}
		} while (_findnext(lf, &file) == 0);
		_findclose(lf);
	}
#else
	DIR* dir;
	struct dirent* ptr;

	if ((dir = opendir(cate_dir.c_str())) == NULL)
	{
		LogIns.error("{0} not found!", cate_dir);
		exit(1);
	}

	while ((ptr = readdir(dir)) != NULL)
	{
		if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0)    ///current dir OR parrent dir
			continue;
			
		std::string otherpath = cate_dir + "/" + ptr->d_name;
		if (DirExists(otherpath))
		{
			if (recursive)
				FileSystem::GetSubDirs(otherpath, dirs, recursive);
			dirs.push_back(otherpath);
		}
	}
	closedir(dir);
#endif
}

std::string FileSystem::ExtractFileName(const std::string& fullPathFileName, bool hasExt)
{
	std::string fileName = fullPathFileName;
	StringUtil::ReplaceAllString(fileName, "\\", "/");
	size_t pos = fileName.find_last_of('/');

	if (pos != fileName.npos) {
		fileName = fileName.substr(pos + 1);
	}

	if (hasExt)
		return fileName;

	return FileSystem::EraseExtName(fileName);

}

std::string FileSystem::EraseExtName(const std::string& fullPathFileName)
{
	size_t pos = fullPathFileName.find_last_of('.');
	if (pos != fullPathFileName.npos) {
		std::string str = fullPathFileName.substr(0, pos);
		return str;
	}
	return fullPathFileName;
}

std::string FileSystem::GetExtName(const std::string& fullPathFileName)
{
	size_t pos = fullPathFileName.find_last_of('.');
	if (pos != fullPathFileName.npos) {
		std::string str = fullPathFileName.substr(pos + 1);
		return str;
	}

	return "";
}

std::string FileSystem::PathSeparator()
{
#ifdef _WIN32
	return "\\";
#else
	return "/";
#endif
}

std::string FileSystem::NativePath(const std::string& path)
{
	std::string targetStr = path;
#ifdef _WIN32
	//StringUtil::ReplaceAllString(targetStr, "/", "\\");
	std::replace(targetStr.begin(), targetStr.end(), '/', '\\');
#else
	//StringUtil::ReplaceAllString(targetStr, "\\", "/");
	std::replace(targetStr.begin(), targetStr.end(), '\\', '/');

#endif
	return targetStr;
}

std::string FileSystem::GetPathName(const std::string& path)
{
	std::string dirPath = FileSystem::NativePath(path);
	if (dirPath.back() == FileSystem::PathSeparator().front())
		dirPath.pop_back();
	auto pos = dirPath.find_last_of(FileSystem::PathSeparator());
	return dirPath.substr(pos + 1, dirPath.length() - pos);
}

std::string FileSystem::CombinePath(const std::string& path1, const std::string& path2)
{
	std::string strPath1 = FileSystem::NativePath(path1);
	std::string strPath2 = FileSystem::NativePath(path2);

	if (strPath1.empty() || strPath1 == ".")
		return strPath2;

	auto vecStr1 = StringUtil::Split(strPath1, FileSystem::PathSeparator());
	auto vecStr2 = StringUtil::Split(strPath2, FileSystem::PathSeparator());

	while (!vecStr1.empty() && vecStr1.back().empty())
	{
		vecStr1.pop_back();
	}

	for (auto& str : vecStr2)
	{
		if (str.empty())
			continue;
		if (str == ".")
			continue;
		if (str == "..")
		{
			vecStr1.pop_back();
			continue;
		}
		vecStr1.push_back(str);
	}

	std::stringstream ss;
	if (strPath1.front() == FileSystem::PathSeparator().front())
	{
//#ifdef _WIN32
//		ss << FileSystem::PathSeparator();
//#endif
		ss << FileSystem::PathSeparator();
	}
	for (size_t i = 0; i < vecStr1.size(); i++)
	{
		if (vecStr1[i].empty())
			continue;
		ss << vecStr1[i];
		if(i != vecStr1.size() - 1)
			ss << FileSystem::PathSeparator();
	}
	//std::string path = ss.str();
	//path.pop_back();
	return ss.str();
}

std::string FileSystem::RelativePath(const std::string& from, const std::string& to)
{
	if (from.empty())
		return to;
	std::string str1 = FileSystem::NativePath(from);
	std::string str2 = FileSystem::NativePath(to);

	std::vector<std::string> vec1 = StringUtil::Split(str1.c_str(), FileSystem::PathSeparator());
	if (vec1.back().empty() || vec1.back() == ".")
		vec1.pop_back();
	std::vector<std::string> vec2 = StringUtil::Split(str2.c_str(), FileSystem::PathSeparator());
	std::vector<std::string>::iterator itfrom = vec1.begin();
	for (; itfrom != vec1.end(); ++itfrom)
	{
		if (vec2.empty())
			break;
		if (StringUtil::IsEqual(itfrom->c_str(),
			vec2.front().c_str()
#ifdef _WIN32
			, true
#else
			, false //unix 区分大小写
#endif
		))
			vec2.erase(vec2.begin());
		else
			break;
	}
	while (itfrom != vec1.end())
	{
		vec2.insert(vec2.begin(), "..");
		++itfrom;
	}

	if (vec2.empty())
		vec2.push_back(".");
	else if (vec2.front() != ".")
		vec2.insert(vec2.begin(), ".");
	itfrom = vec2.begin();
	std::stringstream ss;
	for (; itfrom != vec2.end(); ++itfrom)
	{
		if (itfrom != vec2.begin())
			ss << FileSystem::PathSeparator();
		ss << *itfrom;
	}
	return ss.str();
}

std::string FileSystem::ParentPath(const std::string& path)
{
	if (path.empty())
	{
		LogIns.error("FileSystem::ParentPath empty path!");
		return "";
	}
	std::string parentPath = FileSystem::NativePath(path);
	if (parentPath.back() == FileSystem::PathSeparator().front())
		parentPath.pop_back();
	auto pos = parentPath.find_last_of(FileSystem::PathSeparator());
	return parentPath.substr(0, pos);
}

const std::string FileSystem::GetCurrentDir()
{
	char currentPath[1024];
	memset(currentPath, 0, 1024);
#ifdef WIN32
	GetCurrentDirectory(sizeof(currentPath), currentPath);
#else
	getcwd(currentPath, sizeof(currentPath));
#endif
	return currentPath;
}

bool FileSystem::FileCopy(const std::string& srcFile, const std::string& destFile)
{
	FILE* fp = fopen(srcFile.c_str(), "rb");
	if (!fp)
	{
		return false;
	}
	fseek(fp, 0, SEEK_END);
	long fileLength = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	std::vector<unsigned char> data;
	data.resize(fileLength);
	fread(data.data(), fileLength, 1, fp);
	fclose(fp);
	fp = fopen(destFile.c_str(), "wb");
	if (!fp)
	{
		return false;
	}
	fwrite(data.data(), fileLength, 1, fp);
	fclose(fp);
	return true;
}

bool FileSystem::U8FileCopy(const std::string & srcFile, const std::string & destFile)
{
	std::string srcFileLocal = StringConverter::GetTempUseString(srcFile);
	std::string destFileLocal = StringConverter::GetTempUseString(destFile);
	return FileCopy(srcFileLocal,destFileLocal);
}

bool  FileSystem::TreeCopy(const std::string& sourcePath, const std::string& destPath)
{
	return true;
}

bool FileSystem::WriteFile(const char* filename, const char* buf, unsigned long buf_len)
{
	FILE* fp = fopen(filename, "wb");
	if (fp == nullptr) {
		LogIns.error("Open {0} file for write failed",StringConverter::GetTempU8String(filename));
		return false;
	}
	fwrite(buf, 1, buf_len, fp);
	fclose(fp);
	return true;
}

bool FileSystem::WriteU8File(const std::string & fileName, const std::string & content)
{
	std::string fileLocal = StringConverter::GetTempUseString(fileName);
	return WriteFile(fileLocal.c_str(), content.data(), content.size());
}

bool FileSystem::SaveFile(const std::string& keyOrFileName, const std::string& content, CompressOptions co)
{
	bool result = false;

	std::string filePath = keyOrFileName;
	if (!FileSystem::IsAbsolutePath(filePath))
	{
		LogIns.error("{0} is not absolute path!", filePath);
		return result;
	}

	std::string parent = FileSystem::ParentPath(filePath);
	if (!FileSystem::DirExists(parent))
		FileSystem::CreateDir(parent);

	if (co == CompressOptions::CO_NONE) {
		result = FileSystem::WriteFile(filePath.c_str(), (const char*)(content.data()), content.size());
	}
	//else if (co == CompressOptions::CO_GZIP) {
	//	std::string newKeyOrFileName = filePath + GZIP_TOKEN;
	//	std::string compressedData;
	//	if (GZIP::CompressData(content, compressedData) == GZIP::Result::GZR_FAILED) {
	//		LogIns.error("{0} CompressData failed!", newKeyOrFileName);
	//		return result;
	//	}
	//	result = FileSystem::WriteFile(newKeyOrFileName.c_str(), (const char*)(compressedData.data()), compressedData.size());
	//}
	return result;
}

bool FileSystem::ReadFile(const std::string & keyOrFileName, std::string & content)
{
	bool result = false;

	std::string filePath = keyOrFileName;
	if (!FileSystem::IsAbsolutePath(filePath))
	{
		LogIns.error("{0} is not absolute path!", filePath);
		return result;
	}

	//if (!GZIP::LoadFileAsString(filePath, content))
	//	return false;
	//
	//if (GZIP::IsDataCompressed(content)) {
	//	std::string temp;
	//	if (GZIP::DecompressData(content, temp) == GZIP::Result::GZR_FAILED) {
	//		LogIns.error("{0} CompressData failed!", StringConverter::GetTempU8String(keyOrFileName));
	//		return false;
	//	}
	//	content = temp;
	//}

	return true;
}

bool FileSystem::ReadFile(const char* filename, char** buf, int& len) {
	FILE* fp = fopen(filename, "rb");
	if (fp == nullptr) {
		LogIns.error("Open {0} file for read failed", StringConverter::GetTempU8String(filename));
		return false;
	}
	fseek(fp, 0, SEEK_END);
	len = ftell(fp);
	rewind(fp);

	*buf = (char*)malloc(sizeof(char) * len);
	if (buf == NULL)
	{
		fclose(fp);
		return false;
	}
	size_t result = fread(*buf, 1, len, fp);
	if ((int)result != len)
	{
		fclose(fp);
		return false;
	}
	fclose(fp);
	return true;
}



bool FileSystem::GetFileSize(const std::string& filepath, double& fileSize, std::string& fileSizeUnit)
{
	std::ifstream fin(filepath);
	int size = 0;
	if (!fin.is_open())
	{
		return false;
	}

	fin.seekg(0, std::ios::end);
	size = fin.tellg();
	fin.close();

	const double step = 1024.0;
	fileSize = size / step;
	fileSizeUnit = "KB";
	if (fileSize > step)
	{
		fileSize /= step;
		fileSizeUnit = "MB";
	}
	if (fileSize > step)
	{
		fileSize /= step;
		fileSizeUnit = "GB";
	}
	return true;
}

void fun() {};

std::string FileSystem::GetCurrentModuleDirectory()
{
	std::string path;	
	char szPath[MAX_PATH];
#ifdef WIN32
	MEMORY_BASIC_INFORMATION mbi;
	HMODULE handle = ((::VirtualQuery(FileSystem::GetCurrentModuleDirectory, &mbi, sizeof(mbi)) != 0) ? (HMODULE)mbi.AllocationBase : NULL);
	GetModuleFileNameA(handle, szPath, MAX_PATH);
	path = std::string(szPath);
#else
	Dl_info dl_info;
	dladdr((void*)fun, &dl_info);
	path = (dl_info.dli_fname);
	if (!path.empty())
	{
		if (NULL == realpath(path.c_str(), szPath))
		{
			LogIns.error("GetCurrentModuleDirectory failed: {0}",path);
			return path;
		}
		path = szPath;
	}
#endif
	if (!path.empty())
		return FileSystem::ParentPath(path);
	return path;
}

bool FileSystem::IsAbsolutePath(const std::string& path)
{
	if (path.empty())
		return false;
#ifdef WIN32
	if (path.size() < 2)
		return false;
	return path[0] == '/' || path[1] == ':';
#else
	return path[0] == '/';
#endif
}

bool FileSystem::AdjustFileName(std::string& fileName, const std::string& replaceStr)
{
	if (fileName.empty())
		return false;
	static std::set<char> illegleCharSet{ '!','@','#','$','%','&','*','(',')','=','+','-','{','}','\\',';',':','\"',',','.','/','<','>','?','\'',' ' };
	std::string outString;
	for (auto it = fileName.begin(); it != fileName.end(); ++it)
	{
		if (illegleCharSet.find(*it) != illegleCharSet.end())
			outString += replaceStr;
		else
			outString.push_back(*it);
	}
	StringUtil::ReplaceAllString(outString, "and", replaceStr);
	StringUtil::ReplaceAllString(outString, "or", replaceStr);
	fileName = outString;
	return true;
}

bool FileSystem::AdjustFileNameReName(std::string & fileName)
{
	static std::map<char, char>keyMap = { 
		{'!', 'a'},
		{ '@','b' },
		{ '#','c' },
		{ '$','d' },
		{ '%','e' },
		{ '&','f' },
		{ '*','g' },
		{ '(','h' },
		{ ')','i' },
		{ '=','j' },
		{ '+','k' },
		{ '-','l' },
		{ '{','m' },
		{ '}','n' },
		{ '\\','o' },
		{ ';','p' },
		{ ':','q' },
		{ '\"','r' },
		{ ',','s' },
		{ '.','t' },
		{ '/','u' },
		{ '<','v' },
		{ '>','w' },
		{ '?','x' },
		{ '\'','y' },
		{ ' ','z' } 
	};
	std::string outString;
	for (auto it = fileName.begin(); it != fileName.end(); ++it)
	{
		if (keyMap.find(*it) != keyMap.end())
			outString += keyMap[*it];
		else
			outString.push_back(*it);
	}
	fileName = outString;
	return true;
}

bool FileSystem::IsVaildFileName(const std::string& fileName)
{
	static std::set<char> illegleCharSet{ '!','@','#','$','%','&','*','(',')','=','+','-','{','}','\\',';',':','\"',',','/','<','>','?','\'' };
	for (auto it = fileName.begin(); it != fileName.end(); ++it)
	{
		if (illegleCharSet.find(*it) != illegleCharSet.end())
			return false;
	}

	return true;
}

EPFileEncoding FileSystem::ReadFileEncoding(const std::string& filePath)
{
	if (filePath.empty())
	{
		return FE_NULL;
	}
	std::ifstream fin(filePath, std::ios::binary);
	if (fin.fail())
	{
		return FE_NULL;
	}

	unsigned char  s2;
	fin.read((char*)&s2, sizeof(s2));
	int p = s2 << 8;
	fin.read((char*)&s2, sizeof(s2));
	p |= s2;

	EPFileEncoding enumFE = EPFileEncoding::FE_ANSI;

	switch (p)
	{
	case 0xfffe:  //65534
		enumFE = EPFileEncoding::FE_UNICODE;
		break;
	case 0xfeff://65279
		enumFE = EPFileEncoding::FE_UNICODE_BIG_ENDIAN;
		break;
	case 0xefbb://61371
		enumFE = EPFileEncoding::FE_UTF8_BOM;
		break;
	default:
		if (CheckUtf8WithoutBom(filePath))
			enumFE = EPFileEncoding::FE_UTF8;
		else
			enumFE = EPFileEncoding::FE_ANSI;
	}
	fin.close();

	return enumFE;
}

EPFileEncoding FileSystem::ReadU8FileEncoding(const std::string & filePath)
{
	std::string pathStr = StringConverter::GetTempUseString(filePath);
	return ReadFileEncoding(pathStr);
}

bool FileSystem::ReadU8File(const std::string & filePath, std::string & content)
{
	std::string pathStr = StringConverter::GetTempUseString(filePath);
	return ReadFileString(pathStr,content);
}

bool FileSystem::ReadFileString(const std::string & fileName, std::string & content)
{
	std::fstream fs(fileName, std::ios::in);
	if (!fs.good())
		return false;
	content = std::move(std::string{ std::istreambuf_iterator<char>(fs),std::istreambuf_iterator<char>() });
	fs.close();
	return true;
}

bool FileSystem::IsFileEmpty(const std::string & filename)
{
	if (!FileExists(filename))
		return true;

	char* content = nullptr;
	int len = 0;

	//只取长度，避免分配内存
	FILE* fp = fopen(filename.c_str(), "rb");
	if (fp == nullptr) {
		LogIns.error("Open {0} file for read failed", StringConverter::GetTempU8String(filename));
		return false;
	}
	fseek(fp, 0, SEEK_END);
	len = ftell(fp);
	fclose(fp);

	return len <= 0;

}

void FileSystem::GetU8Files(const std::string& cate_dir, std::vector<std::string>& files, const std::string & format, bool recursive)
{
	if (cate_dir.empty())
	{
		return;
	}
#ifdef WIN32
	std::string dirLocal = StringConverter::GetTempUseString(cate_dir);
	_finddata_t file;
	long long lf = 0;
	std::string filename = dirLocal + osDirSep + "*";
	if ((lf = _findfirst(filename.c_str(), &file)) == -1) {
		LogIns.error("{0} not found!", StringConverter::GetTempU8String(cate_dir));
	}
	else {
		std::string  curpath = dirLocal;
		do
		{
			if (strcmp(file.name, ".") == 0 || strcmp(file.name, "..") == 0)
				continue;
			if ((file.attrib & _A_SUBDIR))
			{
				std::string otherpath = dirLocal + osDirSep + file.name;
				if (recursive)
					FileSystem::GetFiles(otherpath, files, format);
			}
			else {
				std::string filename1 = file.name;
				std::string ext = filename1.substr(filename1.find_last_of(".") + 1);
				transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
				if (ext == format || format == "*") {
					std::string filePath = curpath + osDirSep + file.name;
					filePath = StringConverter::GetTempU8String(filePath);
					files.push_back(filePath);
				}
			}
		} while (_findnext(lf, &file) == 0);
		_findclose(lf);
	}
#else
	GetFiles(cate_dir,files, format, recursive);
#endif
}

bool FileSystem::CheckUtf8WithoutBom(const std::string& content)
{
	//FILE *f = NULL;
	//fopen_s(&f, content.c_str(), "rb");
	FILE* fp = fopen(content.c_str(), "rb");
	if (NULL == fp)
	{
		return false;
	}

	fseek(fp, 0, SEEK_END);
	long lSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);  //或rewind(f);

	char* pBuff = new char[lSize + 1];
	if (nullptr == pBuff)
	{
		LogIns.error("Failed to allocate memory:{0}", content);
		fclose(fp);
		return false;
	}
	memset(pBuff, 0, lSize + 1);
	fread(pBuff, lSize, 1, fp);
	fclose(fp);

	bool bIsUTF8 = FileSystem::IsUTF8Text(pBuff, lSize);
	delete[]pBuff;
	pBuff = NULL;

	return bIsUTF8;
}
bool FileSystem::IsUTF8Text(const std::string& str)
{
	return IsUTF8Text(str.c_str(), str.length());
}

bool FileSystem::IsUTF8Text(const void* pBuffer, long size)
{
	// 注意： 两个字节的，即使符合规则也不一定就是utf8  (gbk中的部分汉字符合 utf8规则)
	bool IsUTF8 = true;
	unsigned char* start = (unsigned char*)pBuffer;
	unsigned char* end = (unsigned char*)pBuffer + size;
	while (start < end)
	{
		if (*start < 0x80) // (10000000): 值小于0x80的为ASCII字符
		{
			start++;
		}
		else if (*start < (0xC0)) // (11000000): 值介于0x80与0xC0之间的为无效UTF-8字符
		{
			IsUTF8 = false;
			break;
		}
		else if (*start < (0xE0)) // (11100000): 此范围内为2字节UTF-8字符
		{
			if (start >= end - 1)
			{
				break;
			}

			//判断二进制前两位是 10 
			if ((start[1] & (0xC0)) != 0x80)
			{
				IsUTF8 = false;
				break;
			}

			start += 2;
		}
		else if (*start < (0xF0)) // (11110000): 此范围内为3字节UTF-8字符
		{
			if (start >= end - 2)
			{
				break;
			}

			if ((start[1] & (0xC0)) != 0x80 || (start[2] & (0xC0)) != 0x80)
			{
				IsUTF8 = false;
				break;
			}

			start += 3;
		}
		else
		{
			IsUTF8 = false;
			break;
		}
	}

	return IsUTF8;
}


void FileSystem::GetU8SubDirs(std::string cate_dir, std::vector<std::string>& dirs,bool recursive)
{
#ifdef WIN32
	std::string dirLocal = StringConverter::GetTempUseString(cate_dir);
	_finddata_t file;
	long long lf = 0;
	std::string filename = dirLocal + PathSeparator() + "*";
	if ((lf = _findfirst(filename.c_str(), &file)) == -1) {
		LogIns.error("{0} not found!", StringConverter::GetTempU8String(cate_dir));
	}
	else {
		std::string  curpath = dirLocal;
		do
		{
			if (strcmp(file.name, ".") == 0 || strcmp(file.name, "..") == 0)
				continue;
			if ((file.attrib & _A_SUBDIR))
			{
				std::string otherpath = dirLocal + PathSeparator() + file.name;
				otherpath = StringConverter::GetTempU8String(otherpath);
				dirs.push_back(otherpath);
			}
		} while (_findnext(lf, &file) == 0);
		_findclose(lf);
	}
#else
	GetSubDirs(cate_dir,dirs,recursive);
#endif
}

bool FileSystem::CheckU8DirExistsOrCreate(const std::string & dir)
{
	std::string dirLocal = StringConverter::GetTempUseString(dir);
	return CheckDirExistsOrCreate(dirLocal);
}

bool FileSystem::U8FileExists(const std::string& fileName)
{
	std::string fileLocal = StringConverter::GetTempUseString(fileName);
	return FileExists(fileLocal);
}

unsigned long long FileSystem::GetFileSize(const std::string & filePath_use)
{
#ifdef _WIN32
	return std::tr2::sys::file_size(filePath_use);
#else
	struct stat statbuf;
	if (stat(filePath_use.c_str(), &statbuf) == 0)
		return statbuf.st_size;
	return 0;
#endif
}

std::string FileSystem::RemoveEndSeparator(const std::string & pathin)
{
	std::string path = FileSystem::NativePath(pathin);
	if (path.size() > 0 && path[path.size() - 1] == FileSystem::PathSeparator()[0]) {
		path = path.substr(0, path.size() - 1);
	}
	return path;
}

