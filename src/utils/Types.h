#pragma once

namespace BBGLOBE {

	enum class DataType {
		eDataTypeUnknown = -1,
		eDataTypeModel = 0,
		eDataTypePoint,
		eDataTypeGIM,
	};

	const std::string osDirSep = "/";


	//设置成员变量的Set和Get方法
	#define MemberFun(type, name)\
		public:\
			inline void Set##name(type v){\
				m##name = v;\
			}\
			inline type Get##name(){\
				return m##name;\
			}
}
