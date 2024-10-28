#include "Image.h"

#include <algorithm>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb_image_resize.h>


//#include <tiffio.hxx>
//
//#include <crunch/crnlib.h>
//#include <crunch/crn_decomp.h>
//#include <crunch/dds_defs.h>

using namespace BBGLOBE;

unsigned char* Image::Load(const std::string& fileName, int& width, int& height, int& channels)
{
	return stbi_load(fileName.c_str(), &width, &height, &channels, 0);
}

unsigned char* Image::Load(const std::string& fileName, int& width, int& height, int& channels, int reqChannels)
{
	return stbi_load(fileName.c_str(), &width, &height, &channels, reqChannels);
}

unsigned char* Image::LoadFromMemory(const unsigned char* pbuffer, int len, int& width, int& height, int& channels)
{
	return stbi_load_from_memory(pbuffer, len, &width, &height, &channels, 0);
}

//unsigned char* Image::LoadTiffFormPath(const std::string& fileName, int& width, int& height, int& channels)
//{
//
//	TIFF* tiff = TIFFOpen(fileName.c_str(), "r");
//	if (tiff == nullptr)
//	{
//		LogIns.error("Read image failed !please Reread!");
//		return nullptr;
//	}
//	TIFFGetField(tiff, TIFFTAG_IMAGEWIDTH, &width);
//	TIFFGetField(tiff, TIFFTAG_IMAGELENGTH, &height);
//	TIFFGetField(tiff, TIFFTAG_SAMPLESPERPIXEL, &channels);
//
//	unsigned char* imageData = nullptr;
//	size_t npixels = width * height;
//	uint32* raster = (uint32*)_TIFFmalloc(npixels * sizeof(uint32));
//	if (raster != NULL)
//	{
//		if (TIFFReadRGBAImage(tiff, width, height, raster, 0))
//		{
//			unsigned char* R = new unsigned char[npixels];
//			unsigned char* G = new unsigned char[npixels];
//			unsigned char* B = new unsigned char[npixels];
//			unsigned char* A = new unsigned char[npixels];
//			for (size_t i = 0; i < npixels; i++)
//			{
//				R[i] = 0x00;
//				G[i] = 0x00;
//				B[i] = 0x00;
//				A[i] = 0x00;
//			}
//			unsigned int* rowPointerToSrc = raster + width * (height - 1);
//			unsigned char* rowPointerToR = R;
//			unsigned char* rowPointerToG = G;
//			unsigned char* rowPointerToB = B;
//			unsigned char* rowPointerToA = A;
//			for (int y = 0; y < height; y++)
//			{
//				unsigned int* colPointerToSrc = rowPointerToSrc;
//				unsigned char* colPointerToR = rowPointerToR;
//				unsigned char* colPointerToG = rowPointerToG;
//				unsigned char* colPointerToB = rowPointerToB;
//				unsigned char* colPointerToA = rowPointerToA;
//				for (int x = 0; x < width; x++)
//				{
//					colPointerToR[0] = (unsigned char)TIFFGetR(colPointerToSrc[0]);
//					colPointerToG[0] = (unsigned char)TIFFGetG(colPointerToSrc[0]);
//					colPointerToB[0] = (unsigned char)TIFFGetB(colPointerToSrc[0]);
//					colPointerToA[0] = (unsigned char)TIFFGetA(colPointerToSrc[0]);
//					colPointerToSrc++;
//					colPointerToR++;
//					colPointerToG++;
//					colPointerToB++;
//					colPointerToA++;
//				}
//				rowPointerToSrc -= width;
//				rowPointerToR += width;
//				rowPointerToG += width;
//				rowPointerToB += width;
//				rowPointerToA += width;
//			}
//			uint32 pixel = height * width * 4;
//			imageData = new unsigned char[pixel];
//			for (uint32 i = 0; i < npixels; i++)
//			{
//				imageData[i * 4] = R[i];
//				imageData[i * 4 + 1] = G[i];
//				imageData[i * 4 + 2] = B[i];
//				imageData[i * 4 + 3] = A[i];
//			}
//			delete R;
//			delete G;
//			delete B;
//			delete A;
//		}
//		_TIFFfree(raster);
//	}
//	TIFFClose(tiff);
//	return imageData;
//}
//
//unsigned char* Image::LoadTiffFormMemory(const std::string& pbuffer, int& width, int& height, int& channels)
//{
//	unsigned char* imageData = nullptr;
//	std::istringstream input_TIFF_stream(pbuffer);
//	TIFF* tif = TIFFStreamOpen("MemTIFF", &input_TIFF_stream);
//	if (tif) {
//		size_t npixels;
//		uint32* raster;
//
//		TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &width);
//		TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &height);
//		TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &channels);
//
//		npixels = width * height;
//		raster = (uint32*)_TIFFmalloc(npixels * sizeof(uint32));
//		if (raster != NULL) {
//			if (TIFFReadRGBAImage(tif, width, height, raster, 0)) {
//				unsigned char* R = new unsigned char[npixels];
//				unsigned char* G = new unsigned char[npixels];
//				unsigned char* B = new unsigned char[npixels];
//				unsigned char* A = new unsigned char[npixels];
//				for (size_t i = 0; i < npixels; i++)
//				{
//					R[i] = 0x00;
//					G[i] = 0x00;
//					B[i] = 0x00;
//					A[i] = 0x00;
//				}
//				unsigned int* rowPointerToSrc = raster + width * (height - 1);
//				unsigned char* rowPointerToR = R;
//				unsigned char* rowPointerToG = G;
//				unsigned char* rowPointerToB = B;
//				unsigned char* rowPointerToA = A;
//				for (int y = 0; y < height; y++)
//				{
//					unsigned int* colPointerToSrc = rowPointerToSrc;
//					unsigned char* colPointerToR = rowPointerToR;
//					unsigned char* colPointerToG = rowPointerToG;
//					unsigned char* colPointerToB = rowPointerToB;
//					unsigned char* colPointerToA = rowPointerToA;
//					for (int x = 0; x < width; x++)
//					{
//						colPointerToR[0] = (unsigned char)TIFFGetR(colPointerToSrc[0]);
//						colPointerToG[0] = (unsigned char)TIFFGetG(colPointerToSrc[0]);
//						colPointerToB[0] = (unsigned char)TIFFGetB(colPointerToSrc[0]);
//						colPointerToA[0] = (unsigned char)TIFFGetA(colPointerToSrc[0]);
//						colPointerToSrc++;
//						colPointerToR++;
//						colPointerToG++;
//						colPointerToB++;
//						colPointerToA++;
//					}
//					rowPointerToSrc -= width;
//					rowPointerToR += width;
//					rowPointerToG += width;
//					rowPointerToB += width;
//					rowPointerToA += width;
//				}
//				uint32 pixel = height * width * 4;
//				imageData = new unsigned char[pixel];
//				for (uint32 i = 0; i < npixels; i++)
//				{
//					imageData[i * 4] = R[i];
//					imageData[i * 4 + 1] = G[i];
//					imageData[i * 4 + 2] = B[i];
//					imageData[i * 4 + 3] = A[i];
//				}
//				delete R;
//				delete G;
//				delete B;
//				delete A;
//			}
//			_TIFFfree(raster);
//
//
//		}
//		TIFFClose(tif);
//	}
//	return imageData;
//}

void ImageWriteFunc(void* context, void* data, int size)
{
	std::string* pStorageStr = static_cast<std::string*>(context);
	pStorageStr->append((const char*)data, size);
}

bool Image::WriteToMemory(const std::string& fileName, int width, int height, int channel, unsigned char* pData, std::string& outImageFileMemory)
{
	std::string ext = fileName.substr(fileName.find_last_of(".") + 1);
	transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
	outImageFileMemory.clear();
	if (ext == "png")
	{
		if (stbi_write_png_to_func(ImageWriteFunc, (void*)&outImageFileMemory, width, height, channel, pData, 0) == 0) {
			return false;
		}
	}
	else if (ext == "jpg")
	{
		//int imgQuality = Config::GetValue_Int("IMGQUALITY", 0);
		int imgQuality = 0;
		if (stbi_write_jpg_to_func(ImageWriteFunc, (void*)&outImageFileMemory, width, height, channel, pData, imgQuality) == 0) {
			return false;
		}
	}
	else if (ext == "tga")
	{
		if (stbi_write_tga_to_func(ImageWriteFunc, (void*)&outImageFileMemory, width, height, channel, pData) == 0) {
			return false;
		}
	}
	else if (ext == "tif") {
		//std::ostringstream output_TIFF_stream;
		//TIFF* imageTest = TIFFStreamOpen("MemTIFF", &output_TIFF_stream);

		//if (imageTest == NULL) {
		//	LogIns.error("Could not open output.tif for writing\n");
		//	return false;
		//}
		//TIFFSetField(imageTest, TIFFTAG_IMAGEWIDTH, width);
		//TIFFSetField(imageTest, TIFFTAG_IMAGELENGTH, height);
		//TIFFSetField(imageTest, TIFFTAG_BITSPERSAMPLE, 8);// bits per channel (sample) 每个通道（样本）
		//TIFFSetField(imageTest, TIFFTAG_SAMPLESPERPIXEL, channel);// samples per pixel 每像素采样
		//TIFFSetField(imageTest, TIFFTAG_ROWSPERSTRIP, height);
		//TIFFSetField(imageTest, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
		//TIFFSetField(imageTest, TIFFTAG_FILLORDER, FILLORDER_MSB2LSB);
		//TIFFSetField(imageTest, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
		//TIFFSetField(imageTest, TIFFTAG_XRESOLUTION, 600.0);
		//TIFFSetField(imageTest, TIFFTAG_YRESOLUTION, 600.0);
		//TIFFSetField(imageTest, TIFFTAG_RESOLUTIONUNIT, RESUNIT_INCH);
		////Write the information to the file
		//TIFFWriteEncodedStrip(imageTest, 0, pData, width * height * channel);
		//TIFFClose(imageTest);

		//outImageFileMemory.clear();
		//outImageFileMemory = output_TIFF_stream.str();
	}
	else {
		//LogIns.error("Export in this format({0}) is not currently supported", ext);
		return false;
	}

	return true;
}

bool Image::Write(const std::string& fileName, int width, int height, int channel, unsigned char* data)
{
	std::string ext = fileName.substr(fileName.find_last_of(".") + 1);
	transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
	if (ext == "png")
	{
		return (stbi_write_png(fileName.c_str(), width, height, channel, data, 0) != 0);
	}
	else if (ext == "jpg")
	{
		//int imgQuality = Config::GetValue_Int("IMGQUALITY", 0);
		int imgQuality = 0;
		return (stbi_write_jpg(fileName.c_str(), width, height, channel, data, imgQuality) != 0);
	}
	else if (ext == "tga")
	{
		return (stbi_write_tga(fileName.c_str(), width, height, channel, data) != 0);
	}
	else if (ext == "tif") {
		//TIFF* imageTest = TIFFOpen(fileName.c_str(), "w");
		//if (imageTest == NULL) {
		//	LogIns.error("Could not open output.tif for writing\n");
		//	return false;
		//}
		//TIFFSetField(imageTest, TIFFTAG_IMAGEWIDTH, width);
		//TIFFSetField(imageTest, TIFFTAG_IMAGELENGTH, height);
		//TIFFSetField(imageTest, TIFFTAG_BITSPERSAMPLE, 8);
		//TIFFSetField(imageTest, TIFFTAG_SAMPLESPERPIXEL, channel);
		//TIFFSetField(imageTest, TIFFTAG_ROWSPERSTRIP, height);
		//TIFFSetField(imageTest, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
		//TIFFSetField(imageTest, TIFFTAG_FILLORDER, FILLORDER_MSB2LSB);
		//TIFFSetField(imageTest, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
		//TIFFSetField(imageTest, TIFFTAG_XRESOLUTION, 600.0);
		//TIFFSetField(imageTest, TIFFTAG_YRESOLUTION, 600.0);
		//TIFFSetField(imageTest, TIFFTAG_RESOLUTIONUNIT, RESUNIT_INCH);
		////Write the information to the file
		//TIFFWriteEncodedStrip(imageTest, 0, data, width * height * channel);

		//TIFFClose(imageTest);
	}
	else {
		//LogIns.error("Export in this format({0}) is not currently supported", ext);
		return false;
	}

	return true;
}
void Image::Resize(unsigned char* srcData, int srcWidth, int srcHeight, unsigned char* destData, int destWidth, int destHeight, int channels)
{
	stbir_resize_uint8(srcData, srcWidth, srcHeight, 0, destData, destWidth, destHeight, 0, channels);
}

unsigned char* Image::Malloc(size_t sz)
{
	return (unsigned char*)stbi__malloc(sz);
}

void Image::Free(unsigned char* data)
{
	stbi_image_free(data);
}

//void Image::ConvertToCrnFormat(const std::string& fileName, const std::string& outFileName, int channels, int compressQuality)
//{
//	//TODO: solve it later.
//}

//void Image::ConvertToDDSFormat(const std::string& fileName, const std::string& outFileName, int channels, int compressQuality)
//{
//	crn_comp_params compressParams;
//	compressParams.m_file_type = cCRNFileTypeDDS;
//
//	int width, height, my_channels;
//	crn_uint32* pData = (crn_uint32*)Load(fileName, width, height, my_channels, 4);
//
//	if (pData) {
//
//		compressParams.m_width = width;
//		compressParams.m_height = height;
//		compressParams.m_file_type = cCRNFileTypeDDS;
//		compressParams.m_dxt_quality = cCRNDXTQualityNormal;
//		compressParams.set_flag(cCRNCompFlagDebugging, false);
//		//compressParams.m_num_helper_threads = Math::Min(Math::Max(std::thread::hardware_concurrency(), 0), cCRNMaxHelperThreads);
//		compressParams.m_num_helper_threads = 0;
//
//		if (channels == 4) {
//			compressParams.m_format = crn_format::cCRNFmtDXT5;
//		}
//		else
//		{
//			compressParams.m_format = crn_format::cCRNFmtDXT1;
//		}
//
//		compressParams.m_quality_level = compressQuality;
//
//		compressParams.m_pImages[0][0] = pData;
//
//		crn_mipmap_params mipParams;
//		mipParams.m_mode = cCRNMipModeUseSourceOrGenerateMips;
//
//		crn_uint32 actual_quality_level;
//		float actual_bitrate;
//		crn_uint32 output_file_size;
//
//		void* pOutput_file_data = crn_compress(compressParams, mipParams, output_file_size, &actual_quality_level, &actual_bitrate);
//		if (!pOutput_file_data) {
//			Free((unsigned char*)pData);
//			LogIns.error("{0} convert error!", StringConverter::GetTempU8String(outFileName));
//			return;
//		}
//
//		FILE* fp = fopen(outFileName.c_str(), "wb");
//		if (fp) {
//			if (fwrite(pOutput_file_data, output_file_size, 1, fp) != 1) {
//				LogIns.error("Failed to write to {0} !", StringConverter::GetTempU8String(outFileName));
//			}
//			fclose(fp);
//		}
//		else {
//			LogIns.error("{0} convert error!", StringConverter::GetTempU8String(outFileName));
//		}
//
//
//		crn_free_block(pOutput_file_data);
//		Free((unsigned char*)pData);
//	}
//
//}

float Image::PackColor(float r, float g, float b)
{
	return r + g*256.0 + b*256.0*256.0;
}
void Image::UnpackColor(float rgb, float& r, float& g, float& b)
{
	b = std::floor(rgb / 256.0 / 256.0);
	g = std::floor((rgb - b*256.0*256.0) / 256.0);
	r = std::floor(rgb - b*256.0*256.0 - g*256.0);
}

//bool Image::ConvertRawDataToDDSFormatToMemory(const std::string& rawData, int width, int height, int channels,
//	int compressQuality, std::string& ddsFileData, size_t helperThreads, size_t maxMipMapCount)
//{
//	// 如果rawData不是rgba格式，扩充成rgba格式
//	std::string sourceRawData;
//	if (channels == 4) {
//		sourceRawData = rawData;
//	}
//	else if (channels == 3) {
//		sourceRawData.resize(width * height * 4);
//		for (int y = 0; y < height; y++) {
//			for (int x = 0; x < width; x++) {
//				sourceRawData[(y * width + x) * 4 + 0] = rawData[(y * width + x) * 3 + 0];
//				sourceRawData[(y * width + x) * 4 + 1] = rawData[(y * width + x) * 3 + 1];
//				sourceRawData[(y * width + x) * 4 + 2] = rawData[(y * width + x) * 3 + 2];
//				sourceRawData[(y * width + x) * 4 + 3] = 255;
//			}
//		}
//	}
//	else {
//		return false;
//	}
//
//	crn_comp_params compressParams;
//	compressParams.m_file_type = cCRNFileTypeDDS;
//
//	crn_uint32* pData = (crn_uint32*)sourceRawData.data();
//
//	if (pData) {
//
//		compressParams.m_width = width;
//		compressParams.m_height = height;
//		compressParams.m_file_type = cCRNFileTypeDDS;
//		compressParams.m_dxt_quality = cCRNDXTQualityNormal;
//		compressParams.set_flag(cCRNCompFlagDebugging, false);
//		//compressParams.m_num_helper_threads = Math::Min(Math::Max(std::thread::hardware_concurrency(), 0), cCRNMaxHelperThreads);
//		compressParams.m_num_helper_threads = helperThreads;
//
//		if (channels == 4) {
//			compressParams.m_format = crn_format::cCRNFmtDXT5;
//		}
//		else
//		{
//			compressParams.m_format = crn_format::cCRNFmtDXT1;
//		}
//
//		compressParams.m_quality_level = compressQuality;
//
//		compressParams.m_pImages[0][0] = pData;
//
//		crn_mipmap_params mipParams;
//		mipParams.m_mode = cCRNMipModeUseSourceOrGenerateMips;
//		mipParams.m_max_levels = maxMipMapCount;
//
//		crn_uint32 actual_quality_level;
//		float actual_bitrate;
//		crn_uint32 output_file_size;
//
//		void* pOutput_file_data = crn_compress(compressParams, mipParams, output_file_size, &actual_quality_level, &actual_bitrate);
//		if (!pOutput_file_data) {
//			LogIns.error("convert error from raw data!");
//			return false;
//		}
//
//		ddsFileData.clear();
//		ddsFileData.assign((const char*)pOutput_file_data, output_file_size);
//
//		crn_free_block(pOutput_file_data);
//
//		return true;
//	}
//	else {
//		LogIns.error("dds convert error!");
//	}
//
//	return false;
//}

//bool Image::ConvertDDSToImage(unsigned char* pData, size_t imgSize, std::string& outImage)
//{
//	// 用法参考 crunch\example1
//	crn_uint8 *pSrc_file_data = pData;
//	crn_uint32 src_file_size= imgSize;
//	crn_texture_desc tex_desc;
//	crn_uint32 *pImages[cCRNMaxFaces * cCRNMaxLevels];
//	if (!crn_decompress_dds_to_images(pSrc_file_data, src_file_size, pImages, tex_desc))
//	{
//		//free(pSrc_file_data);
//		return false;
//	}
//	if (tex_desc.m_faces>0 && tex_desc.m_levels>3)
//	{
//		bool result = true;
//		for (crn_uint32 level_index = 0; level_index < tex_desc.m_levels; level_index++)
//		{
//			int width = std::max(1U, tex_desc.m_width >> level_index);
//			int height = std::max(1U, tex_desc.m_height >> level_index);
//
//			result = result && stbi_write_png_to_func(ImageWriteFunc, (void*)&outImage, width, height, 4, pImages[level_index], 0) != 0;
//		}
//		crn_free_all_images(pImages, tex_desc);
//		return result;
//	}
//	else
//	{
//		return false;
//	}
//}

//bool Image::ConvertToDDSFormatToMemory(const std::string& fileName, int channels, int compressQuality, std::string& ddsFileData, size_t helperThreads)
//{
//	if (fileName.empty())
//	{
//		LogIns.error("dds convert error!");
//		return false;
//	}
//
//	crn_comp_params compressParams;
//	compressParams.m_file_type = cCRNFileTypeDDS;
//
//	int width, height, my_channels;
//	crn_uint32* pData = (crn_uint32*)Load(fileName, width, height, my_channels, 4);
//
//	if (pData) {
//
//		compressParams.m_width = width;
//		compressParams.m_height = height;
//		compressParams.m_file_type = cCRNFileTypeDDS;
//		compressParams.m_dxt_quality = cCRNDXTQualityNormal;
//		compressParams.set_flag(cCRNCompFlagDebugging, false);
//		//compressParams.m_num_helper_threads = Math::Min(Math::Max(std::thread::hardware_concurrency(), 0), cCRNMaxHelperThreads);
//		compressParams.m_num_helper_threads = helperThreads;
//
//		if (channels == 4) {
//			compressParams.m_format = crn_format::cCRNFmtDXT5;
//		}
//		else
//		{
//			compressParams.m_format = crn_format::cCRNFmtDXT1;
//		}
//
//		compressParams.m_quality_level = compressQuality;
//
//		compressParams.m_pImages[0][0] = pData;
//
//		crn_mipmap_params mipParams;
//		mipParams.m_mode = cCRNMipModeUseSourceOrGenerateMips;
//
//		crn_uint32 actual_quality_level;
//		float actual_bitrate;
//		crn_uint32 output_file_size;
//
//		//crn_decompress_dds_to_images()
//
//		void* pOutput_file_data = crn_compress(compressParams, mipParams, output_file_size, &actual_quality_level, &actual_bitrate);
//		if (!pOutput_file_data) {
//			Free((unsigned char*)pData);
//			LogIns.error("convert error from {0}!", fileName.c_str());
//			return false;
//		}
//
//		ddsFileData.clear();
//		ddsFileData.assign((const char*)pOutput_file_data, output_file_size);
//
//		crn_free_block(pOutput_file_data);
//		Free((unsigned char*)pData);
//
//		return true;
//	}
//	else {
//		LogIns.error("dds convert error!");
//	}
//
//	return false;
//}
