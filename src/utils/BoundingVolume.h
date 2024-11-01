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
		BoundingVolumeBox(const BoundingVolumeBox& box);

		// 拷贝构造函数 右值引用方式
		BoundingVolumeBox(BoundingVolumeBox&& box) noexcept;

		bool IsValid() const;

		BoundingVolumeBox& Union(const Point3d& pt);

		BoundingVolumeBox& Union(const BoundingVolumeBox& box);

		Point3d Center3D() const;

		//double Width() const;
		//double Height() const;
		//double Depth() const;

	public:
		// 索引0 1 是X Y坐标的小值; 2 3是 x y 坐标的最大值; 4是z坐标的最小值 5是z坐标的最大值.
		std::vector<double> mData;
	};
}