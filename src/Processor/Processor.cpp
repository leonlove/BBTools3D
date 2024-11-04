#include "Processor.h"
#include "../utils/Types.h"
using namespace BBGLOBE;

Processor::Processor()
{
	mQuadTreeIndex = std::make_shared<QuadTreeIndex>(HIGHEST_LEVEL - LOD_LEVEL_COUNT, HIGHEST_LEVEL);
}

Processor::~Processor()
{

}

bool Processor::ProcessData(Base3DDataPtr currentData)
{
	for (auto modelPtr : currentData->mModelptrArray)
	{
		modelPtr->SetLon(120.0);
		modelPtr->SetLat(30.0);
		modelPtr->SetHeight(0.0);

		// 坐标转换——将网格的顶点转换为以地理坐标为中心点的局部坐标
		modelPtr->CoordinateTransform();

		mQuadTreeIndex->AddModel(modelPtr);
	}
	return true;
}

