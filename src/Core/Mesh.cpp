#include "Mesh.h"
using namespace BBGLOBE;

void Mesh::SetTextureUVArrayPtr(unsigned int num, TextureUVArrayPtr textureUVArrayPtr)
{
	auto iter = mTextureUVPtrMapArray.find(num);
	if (iter == mTextureUVPtrMapArray.end()) {
		mTextureUVPtrMapArray.insert(std::pair<unsigned int, TextureUVArrayPtr>(num, textureUVArrayPtr));
	}
}

TextureUVArrayPtr Mesh::GetTextureUVArrayPtr(unsigned int num)
{
	auto iter = mTextureUVPtrMapArray.find(num);
	if (iter != mTextureUVPtrMapArray.end()) {
		return iter->second;
	}
	return nullptr;
}

BBGLOBE::TexturePtrMapArray BBGLOBE::Mesh::GetTexturePtrMapArray()
{
	return mTexturePtrMapArray;
}

void BBGLOBE::Mesh::SetTexturePtr(unsigned int num, TexturePtr texturePtr)
{
	mTexturePtrMapArray[num] = texturePtr;
}

TexturePtr Mesh::GetTexturePtr(unsigned int num) {
	auto iter = mTexturePtrMapArray.find(num);
	if (iter != mTexturePtrMapArray.end()) {
		return iter->second;
	}
	return nullptr;
}
