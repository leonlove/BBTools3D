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
	* @brief �Զ���������
	*/
	class Mesh {
	public:
		Mesh() {}
		~Mesh() {}


	private:
		MeshType			 mMeshType;				//��������
		std::vector<Point3d> mVertexs;				//���񶥵�����
		std::vector<Point3d> mNormals;				//����������
		UVVector			 mTextureUVs;			//������������
		VertexIndexArray	 mVertexIndexs;			//���񶥵�����
	};

	using MeshPtr = std::shared_ptr<Mesh>;
}