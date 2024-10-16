#pragma once

#include <memory>
#include "DataLoader/DataLoader.h"
#include "Processor/Processor.h"
#include "Converter/Converter.h"
#include "Core/Base3DData.h"
#include "utils/Types.h"

namespace BBGLOBE {
	/*
	** 负责调度数据处理的整个流程
	*/
	class TaskManager {
	public:
		TaskManager();
		~TaskManager();

		bool LoadData(DataType eDataType);
		bool ProcessData();
		bool Converter3DTiles();

	private:
		DataLoader	mDataLoader;			//数据加载类
		Processor	mProcessor;				//数据处理类
		Converter	mConverter;				//数据转换类

		std::shared_ptr<Base3DData> mCurrentData;	//数据加载类加载后的三维数据
	};
}