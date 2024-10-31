#include "BoundingVolume.h"

using namespace BBGLOBE;

BBGLOBE::BoundingVolumeBox::BoundingVolumeBox()
{
	mData.reserve(12);
	mData = { 0 };
}

BBGLOBE::BoundingVolumeBox::BoundingVolumeBox(const std::initializer_list<double>& values)
{
	if (values.size() != 12)
		return;
	mData.reserve(12);
	std::copy(values.begin(), values.end(), std::back_inserter(mData));
}
