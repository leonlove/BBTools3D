#include "StandardModel.h"
#include "Model.h"
using namespace BBGLOBE;

std::shared_ptr<BBGLOBE::Base3DData> StandardModel::TriangleModel()
{
	std::shared_ptr<Model> modelPtr = std::make_shared<Model>();

	MeshPtr mesh = std::make_shared<Mesh>();

	modelPtr->AddMesh(mesh);

	return modelPtr;
}

