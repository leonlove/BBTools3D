#include <iostream>
#include "TaskManager.h"
#include "utils/Types.h"
#include "utils/Log.h"

using namespace std;
using namespace BBGLOBE;

int main(){

	LogIns.initLog();

	//1. 获取数据相关参数；如数据输入路径、输出路径、数据类型
	std::string univer_model_fileName = "D:\\WorkSpace\\SGGlobe\\testdata\\obj\\zhangzhou\\zhangzhou.obj";

	std::string las_fileName = u8"D:\\WorkSpace\\MyGlobe\\02_CPlusPlus\\BBTools3D\\testdata\\las\\dongda1lu\\las(#98-#245)\\las\\dd_132-145_bridges.las";

	TaskManager taskManager;
	if (!taskManager.LoadData(univer_model_fileName, DataType::eDataTypeModel))
	{
		LogIns.error("LoadData {0} failed!", univer_model_fileName);
		return -1;
	}
	//if (!taskManager.LoadData(las_fileName, DataType::eDataTypePoint))
	//{
	//	LogIns.error("LoadData {0} failed!", las_fileName);
	//	return -1;
	//}
	LogIns.info("LoadData Success!");

	if (!taskManager.ProcessData())
	{
		LogIns.info("ProcessData failed!");
		return -1;
	}
	LogIns.info("ProcessData Success!");

	if (!taskManager.Converter3DTiles())
	{
		LogIns.info("Converter3DTiles failed!");
		return -1;
	}
	LogIns.info("Converter3DTiles Success!");

	return 0;
}