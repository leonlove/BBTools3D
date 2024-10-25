#pragma once
#include <memory>
#include <assimp/scene.h>
#include "../Core/Base3DData.h"
#include "../Core/Model.h"

namespace BBGLOBE {
	/*
	** 数据加载类
	** DataLoader模块负责从磁盘或网络加载数据，
	** 并根据提供的数据类型（点云、人工模型、倾斜摄影、BIM等）进行解析，返回一个统一的内部数据结构
	*/
	class DataLoader {
	public:
		DataLoader();
		~DataLoader();

	public:
		/*
		** 加载三维模型，转换为通用的三维数据
		*/
		Base3DDataPtr LoadModel(const std::string& fileName);
		
		/*
		** 加载点云模型，转换为通用的三维数据
		*/
		Base3DDataPtr LoadLas(const std::string& fileName);

	private:
		void ProcessNode(aiNode* node, const aiScene* scene, const aiMatrix4x4& parent, ModelPtr& model);
		void ProcessChildNode(aiNode *node, const aiScene *scene, const aiMatrix4x4& parent, ModelPtr& model);
		MeshPtr ProcessMesh(aiMesh *mesh, aiNode* node, const aiScene *scene, const aiMatrix4x4& mat);
	};
}