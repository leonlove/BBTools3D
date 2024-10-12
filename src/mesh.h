#ifndef MESH_H_
#define MESH_H_

#include <vector>
#include "utils/Point3.h"
/**
* @brief 自定义网格类
*/

class Mesh {
public:
	Mesh(){}
	~Mesh(){}
private:
	std::vector<Point3d> mVertex;
};
#endif