#pragma once

#include <string>
#include <memory>
#include "Model.h"

namespace BBGLOBE {

	class Base3DData {
	public:
		Base3DData() {}
		virtual ~Base3DData() {}

	public:
		ModelPtrArray mModelptrArray;
	};

	using Base3DDataPtr = std::shared_ptr<Base3DData>;
}
