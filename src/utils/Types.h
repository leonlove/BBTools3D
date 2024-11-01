#pragma once

namespace BBGLOBE {

	const std::string osDirSep = "/";

	constexpr double PI = 3.141592653589793238462643383279;

	enum class DataType {
		eDataTypeUnknown = -1,
		eDataTypeModel = 0,
		eDataTypePoint,
		eDataTypeGIM,
	};

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
