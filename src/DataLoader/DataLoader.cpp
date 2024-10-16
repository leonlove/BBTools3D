#include "DataLoader.h"
#include <iostream>
#include "../Core/Model.h"
using namespace std;
using namespace BBGLOBE;

DataLoader::DataLoader()
{

}

DataLoader::~DataLoader()
{

}

std::shared_ptr<Base3DData> DataLoader::LoadModel()
{
	cout << "Loading Model " << endl;
	std::shared_ptr<Base3DData> _data3D = std::make_shared<Model>();
	return _data3D;
}

