#pragma once

#include <list>
#include <vector>
#include <iterator>
#include "MathUtil.h"
#include "Point3.h"

namespace BBGLOBE {
	class BoundingVolumeBox {
	public:
		BoundingVolumeBox();
		BoundingVolumeBox(const std::initializer_list<double>& values);

		// 拷贝构造函数
		BoundingVolumeBox(const BoundingVolumeBox& box) {
			mData = box.mData;
		}

		// 拷贝构造函数 右值引用方式
		BoundingVolumeBox(BoundingVolumeBox&& box) noexcept {
			mData.swap(box.mData);
		}

		bool IsValid() const {
			if (mData.empty() || mData.size() != 6)
				return false;
			for (auto num : mData)
			{
				if (std::isnan(num))
					return false;
			}
			return true;
		}

		BoundingVolumeBox& Union(const Point3d& pt) {
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

		BoundingVolumeBox& Union(const BoundingVolumeBox& box) {
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

		//double Width() const;
		//double Height() const;
		//double Depth() const;

	public:
		// 索引0 1 是X Y坐标的小值; 2 3是 x y 坐标的最大值; 4是z坐标的最小值 5是z坐标的最大值.
		std::vector<double> mData;
	};
}