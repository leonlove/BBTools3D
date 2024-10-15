#ifndef MESH_H_
#define MESH_H_

#include <vector>
#include "utils/Point2.h"
#include "utils/Point3.h"

namespace BBGLOBE {

	/**
	* @brief 自定义网格类
	*/

	using UVVector = std::vector<Point2f>;
	using VertexIndexArray = std::vector<unsigned int>;

	class Mesh {
	public:
		Mesh() {}
		~Mesh() {}


	private:
		std::vector<Point3d> mVertexs;				//网格顶点坐标
		std::vector<Point3d> mNormals;				//网格法线坐标
		UVVector			 mTextureUVs;			//网格纹理坐标
		VertexIndexArray	 mVertexIndexs;			//网格顶点索引
	};
}

#endif