#pragma once
#include <memory>
#include <assimp/scene.h>
#include "../Core/Base3DData.h"
#include "../Core/Model.h"
#include "../Core/Texture.h"
#include "../utils/QuadTreeIndex.h"

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
		// 加载三维模型，转换为通用的三维数据
		Base3DDataPtr LoadModel(const std::string& fileName);
		
		// 加载点云模型，转换为通用的三维数据
		Base3DDataPtr LoadLas(const std::string& fileName);

	private:
		// 解析Assimp的场景结构
		void ProcessNode(aiNode* node, const aiScene* scene, const aiMatrix4x4& parent, ModelPtrArray& outModels);

		// 解析Assimp的场景结构中的网格信息
		MeshPtr ProcessMesh(aiMesh* mesh, aiNode* node, const aiScene* scene, const aiMatrix4x4& mat);

		// 获取网格的顶点信息，如顶点坐标，顶点索引，法线，纹理坐标
		void ProcessMeshVertexsInfo(MeshPtr meshPtr, const aiMatrix4x4 &mat, aiMesh* mesh);

		// 获取网格的纹理信息 
		void ProcessMeshTextureInfo(const aiScene* scene, aiMesh* mesh, MeshPtr meshPtr);

		std::vector<TexturePtr> LoadMaterialTextures(const aiScene *scene, aiMaterial *mat, aiTextureType type, std::string typeName);

	private:
		std::string				mCurrentFileDir;	//读取文件夹的路径
		TexturePtrArray			mTexturePtrArray;	//纹理容器
	};
}