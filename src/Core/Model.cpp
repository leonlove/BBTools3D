#include "Model.h"
using namespace BBGLOBE;

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

