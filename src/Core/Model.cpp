#include "Model.h"
#include "../utils/Log.h"
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

	// 坐标系转换 计算从设置的经纬度定位点转换到center的矩阵
	auto transformMatrix = mModelCoordinateSystemPtr->ENURelativeMatrix(Point3d(mLon,mLat,mHeight),centerPoint3d);
	glm::dmat4 modelMatrix = glm::dmat4{
		transformMatrix.Data[0], transformMatrix.Data[1],transformMatrix.Data[2],transformMatrix.Data[3],
		transformMatrix.Data[4], transformMatrix.Data[5],transformMatrix.Data[6],transformMatrix.Data[7],
		transformMatrix.Data[8], transformMatrix.Data[9],transformMatrix.Data[10],transformMatrix.Data[11],
		transformMatrix.Data[12], transformMatrix.Data[13],transformMatrix.Data[14],transformMatrix.Data[15]
	};

	mLon = centerPoint3d.x();
	mLat = centerPoint3d.y();
	mHeight = centerPoint3d.z();

	glm::dmat3 normalMatrix = glm::transpose(glm::inverse(modelMatrix));

	for (auto mesh : mMeshs)
	{
		VertexArrayPtr vertexArrayPtr = mesh->GetVertexs();
		size_t vecorNumber = vertexArrayPtr->size() / 3;
		NormalArrayPtr normalArrayPtr = mesh->GetNormals();
		for (size_t i = 0; i < vecorNumber; i++)
		{
			glm::dvec3 tempVector;
			tempVector.x = vertexArrayPtr->at(3 * i);
			tempVector.y = vertexArrayPtr->at(3 * i + 1);
			tempVector.z = vertexArrayPtr->at(3 * i + 2);

			glm::dvec4 tempVextex(tempVector, 1.0);
			tempVector = modelMatrix * tempVextex;

			vertexArrayPtr->at(3 * i) = tempVector.x;
			vertexArrayPtr->at(3 * i + 1) = tempVector.y;
			vertexArrayPtr->at(3 * i + 2) = tempVector.z;

			glm::dvec3 pos;
			pos.x = normalArrayPtr->at(3 * i);
			pos.y = normalArrayPtr->at(3 * i + 1);
			pos.z = normalArrayPtr->at(3 * i + 2);

			// 法线变换只需变换矩阵的旋转部分
			pos = glm::normalize(normalMatrix * pos);

			normalArrayPtr->at(3 * i) = pos.x;
			normalArrayPtr->at(3 * i + 1) = pos.y;
			normalArrayPtr->at(3 * i + 2) = pos.z;
		}
		mesh->GetBoudingBox(true);
	}

	LogIns.info("mLon:{0} mLat:{1} mHeight:{2} ",mLon,mLat,mHeight);
}

BBGLOBE::ModelCoordinateSystemPtr BBGLOBE::Model::GetModelCoordinateSystem()
{
	if (mModelCoordinateSystemPtr == nullptr)
		mModelCoordinateSystemPtr = std::make_shared<ModelCoordinateSystem>(mLon, mLat, mHeight);
	return mModelCoordinateSystemPtr;
}


