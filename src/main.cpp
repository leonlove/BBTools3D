#include <iostream>
#include "utils/Point2.h"
#include "utils/Point3.h"
#include "utils/Point2.inl"
using namespace std;
using namespace BBGLOBE;

int main(){
	cout << "Hello World!" << endl;

	Point2d point2d(1,1);
	cout << "point.x = " << point2d.x() << " point.y = " << point2d._v[1] << endl;

	Point2d point2dxx(3,4);
	cout << "point.x = " << point2dxx.x() << " point.y = " << point2dxx._v[1] << endl;

	Point2d point2d3;
	point2d3 += point2dxx;
	cout << "point.x = " << point2d3.x() << " point.y = " << point2d3._v[1] << endl;

	cout << "point2d3.Length = " << point2d3.Length() << endl;


	cout << "point.x = " << point2d3.Normalize().x() << " point.y = " << point2d3.Normalize().y() << endl;
	
	point2d3.Set(10, 10);
	cout << "point.x = " << point2d3.x() << " point.y = " << point2d3._v[1] << endl;


	//point2d3 *= 2;
	//cout << "point.x = " << point2d3.x() << " point.y = " << point2d3._v[1] << endl;

	//point2d3 /= 2;
	//cout << "point.x = " << point2d3.x() << " point.y = " << point2d3._v[1] << endl;

	//if (point2d < point2dxx)
	//	cout << " A point.x = " << point2d.x() << " point.y = " << point2d._v[1] << endl;
	//else
	//	cout << " B point.x = " << point2dxx.x() << " point.y = " << point2dxx._v[1] << endl;

	//Point2d point2d3 = point2d + point2dxx;
	//cout << "point.x = " << point2d3.x() << " point.y = " << point2d3._v[1] << endl;
	//
	//point2d3 = point2d - point2dxx;
	//cout << "point.x = " << point2d3.x() << " point.y = " << point2d3._v[1] << endl;

	//point2d3 = point2d3 * 3;
	//cout << "point.x = " << point2d3.x() << " point.y = " << point2d3._v[1] << endl;

	//point2d3 = point2d3 / 3;
	//cout << "point.x = " << point2d3.x() << " point.y = " << point2d3._v[1] << endl;

	//Point3d point3d;
	//cout << "point.x = " << point3d.x() << " point.y = " << point3d._v[1] << endl;

	return 0;
}