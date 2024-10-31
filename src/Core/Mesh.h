#pragma once

#include <vector>
#include <map>
#include <memory>
#include "../Core/Texture.h"
#include "../utils/Types.h"
#include "../utils/Point2.h"
#include "../utils/Point3.h"
#include "../utils/BoundingVolume.h"

namespace BBGLOBE {

	enum class MeshType
	{
		ePoints,
		eLine,
		eLineLoop,
		eLineStrip,
		eTriangles,
		eTriangleStrip,
		eTriangleFan
	};

	using VertexArray = std::vector<double>;
	using NormalArray = std::vector<float>;
	using VertexIndexArray = std::vector<unsigned int>;
	using TextureUVArray = std::vector<float>;

	using VertexArrayPtr = std::shared_ptr<VertexArray>;
	using NormalArrayPtr = std::shared_ptr<NormalArray>;
	using VertexIndexArrayPtr = std::shared_ptr<VertexIndexArray>;
	using TextureUVArrayPtr = std::shared_ptr<TextureUVArray>;

	using TextureUVPtrMapArray = std::map<unsigned int, TextureUVArrayPtr>;

	/**
	* @brief 自定义网格类
	*/
	class Mesh {
	public:
		Mesh() {
			mMeshType = MeshType::eTriangles;

			mVertexs = std::make_shared<VertexArray>();
			mVertexIndexs = std::make_shared<VertexIndexArray>();
			mNormals = std::make_shared<NormalArray>();
		}
		~Mesh() {}

		MemberFun(VertexArrayPtr, Vertexs);
		MemberFun(NormalArrayPtr, Normals);
		MemberFun(VertexIndexArrayPtr, VertexIndexs);

		// 网格纹理UV坐标的获取和设置
		void SetTextureUVArrayPtr(unsigned int num, TextureUVArrayPtr textureUVArrayPtr);
		TextureUVArrayPtr GetTextureUVArrayPtr(unsigned int num);

		// 设置纹理 一个网格可以包含多个纹理
		TexturePtrMapArray GetTexturePtrMapArray();
		void SetTexturePtr(unsigned int num, TexturePtr texturePtr);
		TexturePtr GetTexturePtr(unsigned int num);

	private:
		std::string						mID;						  //网格唯一标识
		MeshType						mMeshType;					  //网格类型

		VertexArrayPtr					mVertexs;					  //网格顶点坐标
		NormalArrayPtr					mNormals;					  //网格法线坐标
		VertexIndexArrayPtr				mVertexIndexs;				  //网格顶点索引

		//一个网格可能含有多张纹理，所以需要多个网格纹理坐标
		TextureUVPtrMapArray			mTextureUVPtrMapArray;		  //网格纹理UV容器 
		TexturePtrMapArray				mTexturePtrMapArray;		  //网格纹理map容器

		BoundingVolumeBox				mBoundingVolumeBox;			  //网格数据的包围盒
	};

	using MeshPtr = std::shared_ptr<Mesh>;
}