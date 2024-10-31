#pragma once

#include <list>
#include <vector>
#include <iterator>

namespace BBGLOBE {
	class BoundingVolumeBox {
	public:
		BoundingVolumeBox();
		BoundingVolumeBox(const std::initializer_list<double>& values);

	public:
		std::vector<double> mData;
	};
}