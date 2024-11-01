#include "DataLoader.h"
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/gtx/matrix_decompose.hpp>

#include <LasLib/lasreader.hpp>

#include "../Core/Model.h"
#include "../Core/Texture.h"
#include "../utils/StringConverter.h"
#include "../utils/FileSystemUtil.h"
#include "../utils/StringUtil.h"
#include "../utils/Image.h"
#include "../utils/Log.h"

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

	mCurrentFileDir = FileSystem::ParentPath(fileName);

	Assimp::Importer importer;
	const aiScene *scene = nullptr;
	scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_GenNormals);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		return nullptr;

	ModelPtrArray outModelptrArray;

	aiMatrix4x4 baseMatrix;
	ProcessNode(scene->mRootNode, scene, baseMatrix, outModelptrArray);

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

void DataLoader::ProcessNode(aiNode *node, const aiScene *scene, const aiMatrix4x4& parent, ModelPtrArray& outModels)
{
	std::string nodeName = node->mName.length == 0 ? "" : StringConverter::GetTempUseString(std::string(node->mName.C_Str()));
	if (nodeName.empty() && node->mParent != nullptr)
		nodeName = std::string(node->mParent->mName.C_Str());

	const aiMatrix4x4& abs = parent * node->mTransformation;

	unsigned int currentMeshIdx = 0;
	while (currentMeshIdx < node->mNumMeshes)
	{
		unsigned int endMeshIdx = currentMeshIdx + node->mNumMeshes;
		ModelPtr modelPtr = nullptr;
		if (endMeshIdx - currentMeshIdx > 0)
		{
			modelPtr = std::make_shared<Model>();
			modelPtr->SetName(nodeName);
			modelPtr->SetLon(120.0);
			modelPtr->SetLat(30.0);
			modelPtr->SetHeight(0.0);
		}

		for (unsigned int i = currentMeshIdx; i < endMeshIdx; i++)
		{
			unsigned int meshIndex = node->mMeshes[i];
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			MeshPtr meshptr = ProcessMesh(mesh, node, scene, abs);
			if (meshptr == nullptr)
				continue;

			modelPtr->AddMesh(meshptr);
		}
		modelPtr->DirtyBound();

		outModels.push_back(modelPtr);

		currentMeshIdx = endMeshIdx;
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++)
		ProcessNode(node->mChildren[i], scene, abs, outModels);
}


void DataLoader::ProcessMeshVertexsInfo(MeshPtr meshPtr, const aiMatrix4x4 &mat, aiMesh * mesh)
{
	VertexArrayPtr vertexArrayPtr = meshPtr->GetVertexs();
	NormalArrayPtr normalArrayPtr = meshPtr->GetNormals();
	VertexIndexArrayPtr vertexIndexArrayPtr = meshPtr->GetVertexIndexs();
	TextureUVArrayPtr textureUVArrayPtr = meshPtr->GetTextureUVArrayPtr(0);
	if (!textureUVArrayPtr) {
		textureUVArrayPtr = std::make_shared<TextureUVArray>();
	}

	glm::mat4 transformMatrix3 = { mat.a1, mat.a2, mat.a3, mat.a4,
									mat.b1, mat.b2, mat.b3, mat.b4,
									mat.c1, mat.c2, mat.c3, mat.c4,
									mat.d1, mat.d2, mat.d3, mat.d4 };

	// 获取顶点信息 顶点坐标 纹理坐标 法线坐标
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		//positions
		aiVector3D vert = mat * mesh->mVertices[i];

		vertexArrayPtr->push_back(vert.x);
		vertexArrayPtr->push_back(vert.y);
		vertexArrayPtr->push_back(vert.z);

		// normals
		if (mesh->mNormals == nullptr)
		{
			normalArrayPtr->push_back(0.0);
			normalArrayPtr->push_back(0.0);
			normalArrayPtr->push_back(1.0);
		}
		else {
			aiVector3D normalCopy = mesh->mNormals[i];
			aiVector3D norm = aiMatrix3x3(mat) * normalCopy;

			normalArrayPtr->push_back(norm.x);
			normalArrayPtr->push_back(norm.y);
			normalArrayPtr->push_back(norm.z);
		}

		// texture coordinates   texture exist!
		if (mesh->mTextureCoords[0])
		{
			double u = mesh->mTextureCoords[0][i].x;
			double v = mesh->mTextureCoords[0][i].y;
			if (isnan(u)) {
				u = 0.5;
			}
			if (isnan(v)) {
				v = 0.5;
			}
			textureUVArrayPtr->push_back(u);
			textureUVArrayPtr->push_back(v);
		}
		else {
			textureUVArrayPtr->push_back(0.5);
			textureUVArrayPtr->push_back(0.5);
		}
	}
	meshPtr->SetTextureUVArrayPtr(0, textureUVArrayPtr);

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace& face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices / 3; j += 3) {
			unsigned int idx0 = face.mIndices[j + 0];
			unsigned int idx1 = face.mIndices[j + 1];
			unsigned int idx2 = face.mIndices[j + 2];
			vertexIndexArrayPtr->push_back(idx0);
			vertexIndexArrayPtr->push_back(idx1);
			vertexIndexArrayPtr->push_back(idx2);
		}
	}
}

aiTexture* FindTextureFromName(const aiScene *scene, const std::string& filename)
{
	if (filename.empty() || scene == nullptr)
		return nullptr;
	if (filename[0] == '*')
	{
		std::string index = filename.substr(1);
		int i = atoi(index.c_str());
		if (i >= scene->mNumTextures)
			return nullptr;
		return scene->mTextures[i];
	}

	//unsigned char* imageData = nullptr;
	for (unsigned int i = 0; i < scene->mNumTextures; ++i) {
		aiTexture* tex = scene->mTextures[i];
		std::string texFilename(tex->mFilename.C_Str());
#ifndef WIN32
		StringUtil::ReplaceAllString(texFilename, "\\", "/");
#endif
		if (texFilename != filename)
		{
			continue;
		}
		return tex;
	}
	return nullptr;
}

bool TextureFromFile(const char *path, const std::string &directory, const aiScene *scene, TexturePtr texture, bool exportToTarget)
{
	std::string testPathFileName = directory + "/" + std::string(path);
	bool testFileExist = FileSystem::FileExists(testPathFileName);
	std::string extractedFileName = std::string(path);

	if (!testFileExist) {
		extractedFileName = FileSystem::ExtractFileName(std::string(path));
	}

	std::string filename = std::string(extractedFileName);
	aiTexture* aitexture = FindTextureFromName(scene, std::string(path));

	if (filename[0] == '*')
		filename = filename.substr(1);
	filename = StringUtil::ReplaceAllString(filename, "\\", "/");
	std::string filepath = directory + "/" + filename;

	filepath = StringUtil::ReplaceAllString(filepath, "\\", "/");

	size_t posfile = filename.find_last_of(".");
	std::string subsfilename = filename.substr(0, posfile + 1);

	std::string fileExt = filename.substr(posfile + 1);
	transform(fileExt.begin(), fileExt.end(), fileExt.begin(), ::tolower);
	filename = subsfilename + fileExt;

	int width, height, channel;
	width = height = channel = 0;
	unsigned char *data = nullptr;

	if (aitexture == nullptr)
	{
		if (fileExt == "tif")
		{
			//data = Image::LoadTiffFormPath(filepath, width, height, channel);
			//std::vector<std::string> vecFilename = StringUtil::Split(filename, ".");
			//filename = vecFilename[0] + ".png";
		}
		else
		{
			data = Image::Load(filepath, width, height, channel);
		}
	}
	else {
		std::string out_ext = std::string(aitexture->achFormatHint);
		if (out_ext == "tif")
		{
			//std::vector<char> tex_data;
			//tex_data.resize(aitexture->mWidth);
			//memcpy(&tex_data[0], (char*)aitexture->pcData, aitexture->mWidth);
			//std::string textureData(tex_data.data(), tex_data.size());
			//data = Image::LoadTiffFormMemory(textureData, width, height, channel);
			//std::vector<std::string> vecFilename = StringUtil::Split(filename, ".");
			//filename = vecFilename[0] + ".png";
		}
		else
		{
			int length = 0;
			if (aitexture->mHeight == 0)
				length = aitexture->mWidth;
			else {
				//后续扩展
			}
			data = Image::LoadFromMemory(reinterpret_cast<unsigned char*>(aitexture->pcData), length, width, height, channel);

			if (channel == 3)
				filename += ".jpg";
			if (channel == 4)
				filename += ".png";
		}
	}
	size_t pos = filename.find_last_of("/");
	if (pos != filename.npos)
		filename = filename.substr(pos + 1);

	//ImageFilePath = exportFilePath + "/" + filename;

	texture->SetName(filename);
	if (data)
	{
		texture->SetFormat(channel);
		if (channel == 1) {
			//format1 = GL_RED;
		}
		else if (channel == 3) {
			//format1 = GL_RGB;
		}
		else if (channel == 4) {
			//format1 = GL_RGBA;
		}
		texture->SetWidth(width);
		texture->SetHeight(height);
		texture->SetSize(width * height * channel);

		if (true) {
			unsigned char* outputImg = new unsigned char[width*channel*height];
			memcpy(outputImg, data, width*channel*height);
			texture->SetData(outputImg);
		}
	}
	else
	{
		LogIns.warn("Texture failed to load at path: {0}", StringConverter::GetTempU8String(filepath));
		return false;
	}

	//if (exportToTarget) {
	//	bool isImageExist = FileSystem::FileExists(ImageFilePath);
	//	if (!isImageExist) {
	//		Image::Write(ImageFilePath, width, height, channel, data);
	//	}
	//}

	Image::Free(data);

	return true;
}

std::vector<TexturePtr> DataLoader::LoadMaterialTextures(const aiScene *scene, aiMaterial *mat, aiTextureType type, std::string typeName)
{
	std::string textureDirectory = mCurrentFileDir.empty() ? "" : mCurrentFileDir;

	std::vector<TexturePtr> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		// assimp库无法读取带中文路径的纹理，修改之后，统一使用utf-8 ，这里获取到的路径需要转码
		mat->GetTexture(type, i, &str);

		//转换成当前系统风格的路径
		std::string tmp_u8 = std::string(str.C_Str());
		std::string tmp = StringConverter::GetTempUseString(tmp_u8);
		tmp = FileSystem::NativePath(tmp);
		str = tmp.c_str();

		bool skip = false;
		for (unsigned int j = 0; j < mTexturePtrArray.size(); j++)
		{
			if (std::strcmp(mTexturePtrArray[j]->GetName().c_str(), str.C_Str()) == 0)
			{
				textures.push_back(mTexturePtrArray[j]);
				skip = true;
				break;
			}
		}

		if (!skip)
		{
			TexturePtr texture = std::make_shared<Texture>();
			if (!TextureFromFile(str.C_Str(), textureDirectory, scene, texture, true)) {
				return textures;
			}
			str = texture->GetName();
			//str = tmp;
			mat->AddProperty(&str, AI_MATKEY_TEXTURE(type, i));

			textures.push_back(texture);
			mTexturePtrArray.push_back(texture);
		}

	}
	return textures;
}


void DataLoader::ProcessMeshTextureInfo(const aiScene * scene, aiMesh * mesh, MeshPtr meshPtr)
{
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	aiTextureType allTextureTypes[] =
	{
		aiTextureType_DIFFUSE,
		//aiTextureType_SPECULAR,
		//aiTextureType_AMBIENT,
		//aiTextureType_EMISSIVE,
		//aiTextureType_HEIGHT,
		//aiTextureType_NORMALS,
		//aiTextureType_SHININESS,
		//aiTextureType_OPACITY,
		//aiTextureType_DISPLACEMENT,
		//aiTextureType_LIGHTMAP,
		//aiTextureType_REFLECTION,
		//aiTextureType_BASE_COLOR,
		//aiTextureType_NORMAL_CAMERA,
		//aiTextureType_EMISSION_COLOR,
		//aiTextureType_METALNESS,
		//aiTextureType_DIFFUSE_ROUGHNESS,
		//aiTextureType_AMBIENT_OCCLUSION
	};
	int textureTypeCount = sizeof(allTextureTypes) / sizeof(aiTextureType);
	std::map<std::string, int> channelsOfFileNames;
	for (int i = 0; i < textureTypeCount; i++) {
		std::vector<TexturePtr> textures = LoadMaterialTextures(scene, material, allTextureTypes[i], "");
		if (textures.size() > 0)
		{
			for (auto tex : textures) {
				meshPtr->SetTexturePtr(i, tex);
			}
		}
	}
}

MeshPtr DataLoader::ProcessMesh(aiMesh *mesh, aiNode* node, const aiScene *scene, const aiMatrix4x4& mat)
{
	if (mesh->mNumVertices <= 0)
		return nullptr;

	MeshPtr meshPtr = std::make_shared<Mesh>();

	ProcessMeshVertexsInfo(meshPtr, mat, mesh);

	ProcessMeshTextureInfo(scene, mesh, meshPtr);

	return meshPtr;
}