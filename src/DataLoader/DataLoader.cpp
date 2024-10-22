#include "DataLoader.h"
#include <iostream>
#include "../Core/Model.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <LasLib/lasreader.hpp>

using namespace std;
using namespace BBGLOBE;

DataLoader::DataLoader()
{

}

DataLoader::~DataLoader()
{

}

std::shared_ptr<Base3DData> DataLoader::LoadModel(const std::string& fileName)
{
	cout << "Loading Model " << endl;
	std::shared_ptr<Base3DData> _data3D = std::make_shared<Model>();

	Assimp::Importer importer;
	const aiScene *scene = nullptr;
	scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_GenNormals);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		return nullptr;

	return _data3D;
}


std::shared_ptr<Base3DData> DataLoader::LoadLas(const std::string& fileName)
{
	cout << "Loading Las " << endl;
	std::shared_ptr<Base3DData> _data3D = std::make_shared<Model>();

	// 设置LAS读取对象
	LASreadOpener lasreadopener;
	lasreadopener.set_file_name(fileName.c_str());
	LASreader* lasreader = lasreadopener.open();

	// 读取点云数据
	int count = 0;
	while (lasreader->read_point())
	{
		F64 x = lasreader->point.get_x();
		F64 y = lasreader->point.get_y();
		F64 z = lasreader->point.get_z();

		cout << count++ << " x:" << x << " y:" << y << " z:" << z << endl;
	}

	lasreader->close();
	delete lasreader;

	return _data3D;
}
