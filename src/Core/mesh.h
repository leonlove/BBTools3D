#ifndef MESH_H_
#define MESH_H_

#include <vector>
#include "utils/Point2.h"
#include "utils/Point3.h"

namespace BBGLOBE {

	/**
	* @brief �Զ���������
	*/

	using UVVector = std::vector<Point2f>;
	using VertexIndexArray = std::vector<unsigned int>;

	class Mesh {
	public:
		Mesh() {}
		~Mesh() {}


	private:
		std::vector<Point3d> mVertexs;				//���񶥵�����
		std::vector<Point3d> mNormals;				//����������
		UVVector			 mTextureUVs;			//������������
		VertexIndexArray	 mVertexIndexs;			//���񶥵�����
	};
}

#endif