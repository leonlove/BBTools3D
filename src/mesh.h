#ifndef MESH_H_
#define MESH_H_

#include <vector>
#include "utils/Point3.h"
/**
* @brief �Զ���������
*/

class Mesh {
public:
	Mesh(){}
	~Mesh(){}
private:
	std::vector<Point3d> mVertex;
};
#endif