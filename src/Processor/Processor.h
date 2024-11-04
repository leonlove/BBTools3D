#pragma once
#include "../utils/QuadTreeIndex.h"
#include "../Core/Model.h"
#include "../Core/Base3DData.h"

namespace BBGLOBE {
	/*
	**	数据处理类
	**	Processor模块负责对加载后的数据进行预处理操作，
	**  如点云去噪、网格简化、BIM模型的属性优化等
	*/
	class Processor {
	public:
		Processor();
		~Processor();

		bool ProcessData(Base3DDataPtr currentData);
	private:
		QuadTreeIndexPtr		mQuadTreeIndex;
	};
}