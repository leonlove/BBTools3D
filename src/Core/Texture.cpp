#include "Texture.h"
#include "../utils/Image.h"

namespace BBGLOBE {

	
Texture::Texture() {
	//mID = uuid::Generate();
	mData = nullptr;
}

Texture::~Texture() {
	if (mData) {
		delete[] mData;
		mData = nullptr;
	}
}

std::string Texture::GetID() const {
	return mID;
}

void Texture::SetName(const std::string& name){
	mName = name;
}

std::string Texture::GetName() const {
	return mName;
}

void Texture::SetWidth(unsigned int width) {
	mWidth = width;
}

unsigned int Texture::GetWidth() const {
	return mWidth;
}

void Texture::SetHeight(unsigned int height) {
	mHeight = height;
}

unsigned int Texture::GetHeight() const {
	return mHeight;
}

void Texture::SetFormat(unsigned int format) {
	mFormat = format;
}

unsigned int Texture::GetFormat() const {
	return mFormat;
}

void Texture::SetSize(unsigned int size) {
	mSize = size;
}

unsigned int Texture::GetSize() const {
	return mSize;
}

void Texture::SetData(unsigned char* data) {
	mData = data;
}

unsigned char* Texture::GetData() const {
	return mData;
}

std::string Texture::GetFilePath()
{
	return mFilePath;
}

void Texture::SetFilePath(const std::string& filePath)
{
	mFilePath = filePath;
	int width, height, channals;
	unsigned char* pData = Image::Load(mFilePath, width, height, channals);
	if (pData == nullptr)
		return;

	std::string imageFileMemory;
	Image::WriteToMemory(mFilePath, width, height, channals, pData, imageFileMemory);

	mWidth = imageFileMemory.size();
	mHeight = 0;
	mFormat = channals;
	mSize = imageFileMemory.size();
	mData = new unsigned char[imageFileMemory.size()];
	memcpy(mData, imageFileMemory.data(), mSize);
	Image::Free(pData);
}



}// end of namespace EPGIS_3D
