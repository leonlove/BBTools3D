#pragma once
#include "../utils/QuadTreeIndex.h"
#include "../utils/Types.h"

namespace BBGLOBE {
	/*
	** 数据转换类
	** Converter模块负责将预处理后的数据转换为3D Tiles格式，
	** 包括生成tileset.json文件、分块管理和层级细化（LOD）
	*/
	class Converter {
	public:
		Converter();
		~Converter();
	private:
	};
}