#include <iostream>
#include "TaskManager.h"
#include "utils/Types.h"

using namespace std;
using namespace BBGLOBE;

int main(){
	//1. 获取数据相关参数；如数据输入路径、输出路径、数据类型
	std::string univer_model_fileName = "D:\\WorkSpace\\SGGlobe\\testdata\\obj\\zhangzhou\\zhangzhou.obj";

	std::string las_fileName = "D:\\WorkSpace\\SGGlobe\\testdata\\obj\\zhangzhou\\zhangzhou.obj";

	TaskManager taskManager;
	if (!taskManager.LoadData(univer_model_fileName,DataType::eDataTypeModel))
	{
		cout << "LoadData " << univer_model_fileName << " failed!" << endl;
		return -1;
	}
	if (!taskManager.LoadData(las_fileName, DataType::eDataTypePoint))
	{
		cout << "LoadData " << las_fileName << " failed!" << endl;
		return -1;
	}
	cout << "LoadData Success" << endl;

	if (!taskManager.ProcessData())
	{
		cout << "ProcessData failed" << endl;
		return -1;
	}
	cout << "ProcessData Success" << endl;

	if (!taskManager.Converter3DTiles())
	{
		cout << "Converter3DTiles failed" << endl;
		return -1;
	}
	cout << "Converter3DTiles Success" << endl;

	return 0;
}