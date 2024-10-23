#pragma once
#ifndef POINT3_H_
#define POINT3_H_

namespace BBGLOBE {

	/** 3D点坐标，模板类，可表示三维空间坐标的点，向量, 也可进行三维的数值运算 */
	template<typename T>
	class Point3
	{
	public:
		Point3(void)
		{
			_v[0] = 0;
			_v[1] = 0;
			_v[2] = 0;
		}
		Point3(const Point3 &obj)
		{
			_v[0] = obj._v[0];
			_v[1] = obj._v[1];
			_v[2] = obj._v[2];
		}
		Point3(T x, T y, T z)
		{
			_v[0] = x;
			_v[1] = y;
			_v[2] = z;
		}

	public:
		T  &x(void) { return _v[0]; }
		T  &y(void) { return _v[1]; }
		T  &z(void) { return _v[2]; }
		const T  &x(void) const { return _v[0]; }
		const T  &y(void) const { return _v[1]; }
		const T  &z(void) const { return _v[2]; }
		const Point3 &operator=(const Point3 &point)
		{
			if (this == &point)  return *this;
			_v[0] = point._v[0];
			_v[1] = point._v[1];
			_v[2] = point._v[2];
			return *this;
		}
		bool operator==(const Point3 &point) const
		{
			if (this == &point)  return true;

			//if (!Math::IsFloatEqual(_v[0], point._v[0])) return false;
			//if (!Math::IsFloatEqual(_v[1], point._v[1])) return false;
			//if (!Math::IsFloatEqual(_v[2], point._v[2])) return false;

			return true;
		}
		bool operator!=(const Point3 &point) const
		{
			return !operator==(point);
		}

		bool operator < (const Point3 &point) const
		{
			return _v[0] < point._v[0] || (Math::IsFloatEqual(_v[0], point._v[0]) && _v[1] < point._v[1])
				|| (Math::IsFloatEqual(_v[0], point._v[0]) && Math::IsFloatEqual(_v[1], point._v[1]) && _v[2] < point._v[2]);
		}

		Point3 operator - (const Point3& rhs) const
		{
			return Point3(_v[0] - rhs._v[0], _v[1] - rhs._v[1], _v[2] - rhs._v[2]);
		}
		Point3 operator + (const Point3& rhs) const
		{
			return Point3(_v[0] + rhs._v[0], _v[1] + rhs._v[1], _v[2] + rhs._v[2]);
		}
		Point3 operator * (T rhs) const
		{
			return Point3(_v[0] * rhs, _v[1] * rhs, _v[2] * rhs);
		}

		const Point3& operator *= (T rhs)
		{
			_v[0] *= rhs;
			_v[1] *= rhs;
			_v[2] *= rhs;
			return *this;
		}

		Point3 operator / (T rhs) const
		{
			return Point3(_v[0] / rhs, _v[1] / rhs, _v[2] / rhs);
		}

		const Point3& operator /= (T rhs)
		{
			_v[0] /= rhs;
			_v[1] /= rhs;
			_v[2] /= rhs;
			return *this;
		}

		const Point3& operator += (const Point3& rhs)
		{
			_v[0] += rhs._v[0];
			_v[1] += rhs._v[1];
			_v[2] += rhs._v[2];
			return *this;
		}

		const Point3& operator -= (const Point3& rhs)
		{
			_v[0] -= rhs._v[0];
			_v[1] -= rhs._v[1];
			_v[2] -= rhs._v[2];
			return *this;
		}

		Point3 operator - () const
		{
			return Point3(-_v[0], -_v[1], -_v[2]);
		}

		/** \brief 3D坐标的长度 */
		double length() const
		{
			return sqrt(_v[0] * _v[0] + _v[1] * _v[1] + _v[2] * _v[2]);
		}

		/** \brief 归一化点坐标 */
		Point3 normalize()
		{
			T norm = Point3::length();
			Point3 inv;
			if (norm > 0.0)
			{
				inv = 1.0 / norm;
				_v[0] *= inv;
				_v[1] *= inv;
				_v[2] *= inv;
			}
			return inv;
		}

		/** \brief 设置x,y,z轴的分量
		*  \param x x轴方向值
		*  \param y y轴方向值
		*  \param z z轴方向值  */
		void set(T x, T y, T z)
		{
			_v[0] = x;
			_v[1] = y;
			_v[2] = z;
		}

		/** \brief 数学运算——点乘
		*  \param[Point3] 三维点
		*  \return[T] 点乘值  */
		T Dot(const Point3& other) const
		{
			return _v[0] * other._v[0] + _v[1] * other._v[1] + _v[2] * other._v[2];
		}

		/** \brief 叉乘
		*  \param[Point3] 三维点
		*  \return[Point3] 叉乘值  */
		Point3 CrossProduct(const Point3& other) const
		{
			return Point3(
				_v[1] * other._v[2] - _v[2] * other._v[1],
				_v[2] * other._v[0] - _v[0] * other._v[2],
				_v[0] * other._v[1] - _v[1] * other._v[0]
			);
		}
	public:
		T   _v[3];
	};

	typedef Point3<int>    Point3i;
	typedef Point3<float>  Point3f;
	typedef Point3<double> Point3d;
}

#endif
