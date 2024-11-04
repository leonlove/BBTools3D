#include "TaskManager.h"

using namespace BBGLOBE;

TaskManager::TaskManager()
{

}

BBGLOBE::TaskManager::~TaskManager()
{

}

bool BBGLOBE::TaskManager::LoadData(std::string fileName, DataType eDataType)
{
	switch (eDataType)
	{
	case BBGLOBE::DataType::eDataTypeModel:
		mCurrentData = mDataLoader.LoadModel(fileName);
		break;
	case BBGLOBE::DataType::eDataTypePoint:
		mCurrentData = mDataLoader.LoadLas(fileName);
		break;
	case BBGLOBE::DataType::eDataTypeGIM:
		break;
	default:
		break;
	}

	return true;
}

bool BBGLOBE::TaskManager::ProcessData()
{
	mProcessor.ProcessData(mCurrentData);


	return true;
}

bool BBGLOBE::TaskManager::Converter3DTiles()
{
	return true;
}

