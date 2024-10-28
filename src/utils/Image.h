/*********************************************************************
 * @file   Image.h
 * @brief  Image类封装
 *
 * @author BB
 * @date   August 2024
 *********************************************************************/
#pragma once

#include <string>

namespace BBGLOBE {

	class Image
	{
	public:
		static unsigned char* Load(const std::string& fileName, int& width, int& height, int& channels);
		static unsigned char* Load(const std::string& fileName, int& width, int& height, int& channels, int reqChannels);
		static unsigned char* LoadFromMemory(const unsigned char* pbuffer, int len, int& width, int& height, int& channels);
		//static unsigned char* LoadTiffFormPath(const std::string& fileName, int& width, int& height, int& channels);
		//static unsigned char* LoadTiffFormMemory(const std::string& pbuffer, int& width, int& height, int& channels);

		static bool Write(const std::string& fileName, int width, int height, int channel, unsigned char* data);
		static bool WriteToMemory(const std::string& fileName, int width, int height, int channel, unsigned char* pData, std::string& outImageFileMemory);

		static void Resize(unsigned char* srcData, int srcWidth, int srcHeight, unsigned char* destData, int destWidth, int destHeight, int channels);

		static unsigned char* Malloc(size_t sz);
		static void Free(unsigned char* data);

		//static void ConvertToCrnFormat(const std::string& fileName, const std::string& outFileName, int channels, int compressQuality);
		//static void ConvertToDDSFormat(const std::string& fileName, const std::string& outFileName, int channels, int compressQuality);
		//static bool ConvertToDDSFormatToMemory(const std::string& fileName, int channels, int compressQuality, std::string& ddsFileData, size_t helperThreads = 0);
		//static bool ConvertRawDataToDDSFormatToMemory(const std::string& rawData, int width, int height, int channels,
		//	int compressQuality, std::string& ddsFileData, size_t helperThreads = 0, size_t maxMipMapCount = 16);

		//static bool ConvertDDSToImage(unsigned char* pData, size_t imgSize, std::string& outImage);

		static float PackColor(float r, float g, float b);
		static void  UnpackColor(float rgb, float& r, float& g, float& b);
	};

}