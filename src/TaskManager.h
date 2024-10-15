#pragma once

#include "DataLoader/DataLoader.h"
#include "Processor/Processor.h"
#include "Converter/Converter.h"

namespace BBGLOBE {
	/*
	** 负责调度数据处理的整个流程
	*/
	class TaskManager {
	public:
		TaskManager();
		~TaskManager();

	private:
		DataLoader	mDataLoader;
		Processor	mProcessor;
		Converter	mConverter;
	};
}