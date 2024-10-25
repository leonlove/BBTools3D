#pragma once

#include <string>
#include <vector>
#include "mesh.h"
#include "Base3DData.h"

namespace BBGLOBE {

	/**
	* @brief 自定义三维数据基类
	*/
	class Model: public Base3DData {
	public:
		Model() {}
		virtual ~Model() {}

		void SetName(const std::string& val) {
			mName = val;
		}
		/*
		** 返回数据类型
		*/
		std::string getType() const override {
			return "Model";
		}

		void AddMesh(MeshPtr mesh);

	private:
		std::string			 mName;			// 模型名称
		std::vector<MeshPtr> mMeshs;
	};

	using ModelPtr = std::shared_ptr<Model>;
}
