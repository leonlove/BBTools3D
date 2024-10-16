#pragma once

#include <string>
#include "Base3DData.h"

namespace BBGLOBE {

	/**
	* @brief 自定义三维数据基类
	*/
	class Model: public Base3DData {
	public:
		Model() {}
		virtual ~Model() {}

		/*
		** 返回数据类型
		*/
		std::string getType() const override {
			return "Model";
		}
	};
}
