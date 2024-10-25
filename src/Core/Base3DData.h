#pragma once

#include <string>
#include <memory>

namespace BBGLOBE {

	/**
	* @brief 自定义三维数据基类
	*/
	class Base3DData {
	public:
		Base3DData() {}
		virtual ~Base3DData() {}

		/*
		** 返回数据类型
		*/
		virtual std::string getType() const = 0;
	};

	using Base3DDataPtr = std::shared_ptr<Base3DData>;
}
