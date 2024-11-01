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

TexturePtrMapArray Mesh::GetTexturePtrMapArray()
{
	return mTexturePtrMapArray;
}

void Mesh::SetTexturePtr(unsigned int num, TexturePtr texturePtr)
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

BoundingVolumeBox Mesh::GetBoudingBox(bool force /*= false*/)
{
	if (!force && (mBoundingVolumeBox.IsValid() || !mVertexs->empty()))
	{
		return mBoundingVolumeBox;
	}
	mBoundingVolumeBox.mData.clear();
	size_t pointCount = mVertexs->size() / 3;
	for (size_t i = 0; i < pointCount; i++)
	{
		mBoundingVolumeBox.Union(Point3d(mVertexs->at(i * 3), mVertexs->at(i * 3 + 1), mVertexs->at(i * 3 + 2)));
	}
	return mBoundingVolumeBox;
}