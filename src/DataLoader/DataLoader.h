#pragma once

namespace BBGLOBE {
	/*
	** 数据加载类
	** DataLoader模块负责从磁盘或网络加载数据，
	** 并根据提供的数据类型（点云、人工模型、倾斜摄影、BIM等）进行解析，返回一个统一的内部数据结构
	*/
	class DataLoader {
	public:
		DataLoader();
		~DataLoader();
	private:

	};
}