#pragma once
#include <memory>
#include <string>
#include "Base3DData.h"

namespace BBGLOBE {
	class StandardModel {
	public:
		static std::shared_ptr<Base3DData> TriangleModel();
	};
}
