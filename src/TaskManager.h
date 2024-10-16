#pragma once

#include <memory>
#include "DataLoader/DataLoader.h"
#include "Processor/Processor.h"
#include "Converter/Converter.h"
#include "Core/Base3DData.h"
#include "utils/Types.h"

namespace BBGLOBE {
	/*
	** ����������ݴ������������
	*/
	class TaskManager {
	public:
		TaskManager();
		~TaskManager();

		bool LoadData(DataType eDataType);
		bool ProcessData();
		bool Converter3DTiles();

	private:
		DataLoader	mDataLoader;			//���ݼ�����
		Processor	mProcessor;				//���ݴ�����
		Converter	mConverter;				//����ת����

		std::shared_ptr<Base3DData> mCurrentData;	//���ݼ�������غ����ά����
	};
}