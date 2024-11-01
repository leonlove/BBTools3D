#include "Model.h"
#include "..\utils\Log.h"
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
	if (mModelCoordinateSystemPtr == nullptr)
		GetModelCoordinateSystem();

	auto centerPoint3d = mBoundingBox.Center3D();

	mModelCoordinateSystemPtr->Model2Geo(centerPoint3d.x(), centerPoint3d.y(), centerPoint3d.z());

	mLon = centerPoint3d.x();
	mLat = centerPoint3d.y();
	mHeight = centerPoint3d.z();

	LogIns.info("mLon:{0} mLat:{1} mHeight:{2} ",mLon,mLat,mHeight);
}

BBGLOBE::ModelCoordinateSystemPtr BBGLOBE::Model::GetModelCoordinateSystem()
{
	if (mModelCoordinateSystemPtr == nullptr)
		mModelCoordinateSystemPtr = std::make_shared<ModelCoordinateSystem>(mLon, mLat, mHeight);
	return mModelCoordinateSystemPtr;
}


