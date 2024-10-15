#ifndef MESH_H_
#define MESH_H_

#include <vector>
#include "utils/Point3.h"

using namespace BBGLOBE;

/**
* @brief �Զ���������
*/

class Mesh {
public:
	Mesh(){}
	~Mesh(){}
private:
	std::vector<Point3d> mVertices;
	std::vector<Point3d> mNormals;
};
#endif