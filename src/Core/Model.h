#pragma once

#include <string>
#include <vector>
#include "mesh.h"
#include "Base3DData.h"

namespace BBGLOBE {

	/**
	* @brief �Զ�����ά���ݻ���
	*/
	class Model: public Base3DData {
	public:
		Model() {}
		virtual ~Model() {}

		/*
		** ������������
		*/
		std::string getType() const override {
			return "Model";
		}

		void AddMesh(MeshPtr mesh);

	private:
		std::vector<MeshPtr> mMeshs;
	};

	using ModelPtr = std::shared_ptr<Model>;
}
