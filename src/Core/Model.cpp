#include "Model.h"
using namespace BBGLOBE;


Model::Model()
{
	mModelCoordinateSystemPtr = nullptr;
}

Model::~Model()
{

}

void Model::AddMesh(MeshPtr mesh)
{
	mMeshs.push_back(mesh);
}

void Model::DirtyBound()
{
	mBoundingBox.mData.clear();
	for (auto mesh : mMeshs)
	{
		mBoundingBox.Union(mesh->GetBoudingBox(true));
	}
}

void Model::CoordinateTransform()
{

}

BBGLOBE::ModelCoordinateSystemPtr BBGLOBE::Model::GetModelCoordinateSystem()
{
	if (mModelCoordinateSystemPtr == nullptr)
		mModelCoordinateSystemPtr = std::make_shared<ModelCoordinateSystem>(mLon, mLat, mHeight);
	return mModelCoordinateSystemPtr;
}


