#pragma once

#include <string>

namespace BBGLOBE {

	/**
	* @brief 三维数据点云类
	*/
	class PointCloud {
	public:
		PointCloud() {}
		virtual ~PointCloud() {}

		///*
		//** 返回数据类型
		//*/
		//std::string getType() const override {
		//	return "PointCloud";
		//}
	};
}
