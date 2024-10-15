#pragma once

#include "DataLoader/DataLoader.h"
#include "Processor/Processor.h"
#include "Converter/Converter.h"

namespace BBGLOBE {
	/*
	** ����������ݴ������������
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