#pragma once

#include <string>

namespace BBGLOBE {

	/**
	* @brief �Զ�����ά���ݻ���
	*/
	class Base3DData {
	public:
		Base3DData() {}
		virtual ~Base3DData() {}

		/*
		** ������������
		*/
		virtual std::string getType() const = 0;
	};
}
