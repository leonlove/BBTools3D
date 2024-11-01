#include "BoundingVolume.h"

using namespace BBGLOBE;

BBGLOBE::BoundingVolumeBox::BoundingVolumeBox()
{
	mData.reserve(12);
	mData = { 0 };
}


BBGLOBE::BoundingVolumeBox::BoundingVolumeBox(BoundingVolumeBox&& box) noexcept
{
	mData.swap(box.mData);
}

BBGLOBE::BoundingVolumeBox::BoundingVolumeBox(const BoundingVolumeBox& box)
{
	mData = box.mData;
}

BBGLOBE::BoundingVolumeBox::BoundingVolumeBox(const std::initializer_list<double>& values)
{
	if (values.size() != 12)
		return;
	mData.reserve(12);
	std::copy(values.begin(), values.end(), std::back_inserter(mData));
}

bool BBGLOBE::BoundingVolumeBox::IsValid() const
{
	if (mData.empty() || mData.size() != 6)
		return false;
	for (auto num : mData)
	{
		if (std::isnan(num))
			return false;
	}
	return true;
}

BBGLOBE::BoundingVolumeBox& BBGLOBE::BoundingVolumeBox::Union(const Point3d& pt)
{
	if (!IsValid()) {
		mData = std::vector<double>{ pt.x(), pt.y(), pt.x(), pt.y(), pt.z(), pt.z() };
		return *this;
	}
	mData[0] = Math::Min(mData[0], pt.x());
	mData[1] = Math::Min(mData[1], pt.y());
	mData[4] = Math::Min(mData[4], pt.z());

	mData[2] = Math::Max(mData[2], pt.x());
	mData[3] = Math::Max(mData[3], pt.y());
	mData[5] = Math::Max(mData[5], pt.z());

	return *this;
}

BBGLOBE::BoundingVolumeBox& BBGLOBE::BoundingVolumeBox::Union(const BoundingVolumeBox& box)
{
	// 先做安全检测
	if (!box.IsValid()) return *this;
	if (!IsValid()) {
		mData = box.mData;
		return *this;
	}
	mData[0] = Math::Min(mData[0], box.mData[0]);
	mData[1] = Math::Min(mData[1], box.mData[1]);
	mData[4] = Math::Min(mData[4], box.mData[4]);

	mData[2] = Math::Max(mData[2], box.mData[2]);
	mData[3] = Math::Max(mData[3], box.mData[3]);
	mData[5] = Math::Max(mData[5], box.mData[5]);

	return *this;
}

BBGLOBE::Point3d BBGLOBE::BoundingVolumeBox::Center3D() const
{
	if (!IsValid())
		return Point3d(0, 0, 0);
	return Point3d{ (mData[0] + mData[2]) / 2,(mData[1] + mData[3]) / 2, (mData[4] + mData[5]) / 2 };
}
