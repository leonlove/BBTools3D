#pragma once

#include <string>
#include "Base3DData.h"

namespace BBGLOBE {

	/**
	* @brief ��ά���ݵ�����
	*/
	class PointCloud: public Base3DData {
	public:
		PointCloud() {}
		virtual ~PointCloud() {}

		/*
		** ������������
		*/
		std::string getType() const override {
			return "PointCloud";
		}
	};
}
