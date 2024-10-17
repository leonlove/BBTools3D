#pragma once

#include <vector>
#include "../utils/Point2.h"
#include "../utils/Point3.h"

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

	using UVVector = std::vector<Point2f>;
	using VertexIndexArray = std::vector<unsigned int>;

	/**
	* @brief 自定义网格类
	*/
	class Mesh {
	public:
		Mesh() {}
		~Mesh() {}


	private:
		MeshType			 mMeshType;				//网格类型
		std::vector<Point3d> mVertexs;				//网格顶点坐标
		std::vector<Point3d> mNormals;				//网格法线坐标
		UVVector			 mTextureUVs;			//网格纹理坐标
		VertexIndexArray	 mVertexIndexs;			//网格顶点索引
	};

	using MeshPtr = std::shared_ptr<Mesh>;
}