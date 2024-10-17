#pragma once
#include <memory>
#include "../Core/Base3DData.h"

namespace BBGLOBE {
	/*
	** ���ݼ�����
	** DataLoaderģ�鸺��Ӵ��̻�����������ݣ�
	** �������ṩ���������ͣ����ơ��˹�ģ�͡���б��Ӱ��BIM�ȣ����н���������һ��ͳһ���ڲ����ݽṹ
	*/
	class DataLoader {
	public:
		DataLoader();
		~DataLoader();

	public:
		std::shared_ptr<Base3DData> LoadModel(const std::string& fileName);
	};
}