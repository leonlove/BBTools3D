#pragma once

#include <string>
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
	};
}
