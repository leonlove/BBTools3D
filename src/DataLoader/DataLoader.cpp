#include "DataLoader.h"
#include <iostream>
#include "../Core/Model.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "glm/gtx/matrix_decompose.hpp"
#include <LasLib/lasreader.hpp>

using namespace std;
using namespace BBGLOBE;

DataLoader::DataLoader()
{

}

DataLoader::~DataLoader()
{

}

Base3DDataPtr DataLoader::LoadModel(const std::string& fileName)
{
	cout << "Loading Model " << endl;
	std::shared_ptr<Base3DData> _data3D = std::make_shared<Model>();

	Assimp::Importer importer;
	const aiScene *scene = nullptr;
	scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_GenNormals);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		return nullptr;

	ModelPtr modelptr = nullptr;

	aiMatrix4x4 baseMatrix;
	ProcessNode(scene->mRootNode, scene, baseMatrix, modelptr);

	return _data3D;
}


Base3DDataPtr DataLoader::LoadLas(const std::string& fileName)
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

void DataLoader::ProcessNode(aiNode* node, const aiScene* scene, const aiMatrix4x4& parent, ModelPtr& model) {

	if (node->mNumChildren <= 0)
		return;

	model = std::make_shared<Model>();
	model->SetName(node->mName.C_Str());
	const aiMatrix4x4& abs = parent * node->mTransformation;
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessChildNode(node->mChildren[i], scene, abs, model);
	}
}

void DataLoader::ProcessChildNode(aiNode *node, const aiScene *scene, const aiMatrix4x4& parent, ModelPtr& model)
{
	const aiMatrix4x4& abs = parent * node->mTransformation;
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		int index = node->mMeshes[i];
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		MeshPtr meshptr = ProcessMesh(mesh, node, scene, abs);
		if (meshptr == nullptr)
			continue;

		model->AddMesh(meshptr);
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++)
		ProcessChildNode(node->mChildren[i], scene, abs, model);
}

MeshPtr DataLoader::ProcessMesh(aiMesh *mesh, aiNode* node, const aiScene *scene, const aiMatrix4x4& mat)
{
	if (mesh->mNumVertices <= 0)
		return nullptr;

	glm::mat4 transformMatrix3 = { mat.a1, mat.a2, mat.a3, mat.a4,
		mat.b1, mat.b2, mat.b3, mat.b4,
		mat.c1, mat.c2, mat.c3, mat.c4,
		mat.d1, mat.d2, mat.d3, mat.d4 };

	return nullptr;
}