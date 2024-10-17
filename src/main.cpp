#include <iostream>
#include "TaskManager.h"
#include "utils/Types.h"

using namespace std;
using namespace BBGLOBE;

int main(){
	//1. ��ȡ������ز���������������·�������·������������
	std::string fileName = "";

	TaskManager taskManager;
	if (!taskManager.LoadData(fileName,DataType::eDataTypeModel))
	{
		cout << "LoadData failed!" << endl;
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