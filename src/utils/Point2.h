#pragma once
#ifndef POINT2_H_
#define POINT2_H_

namespace BBGLOBE {

	/** 2D点坐标，模板类，可表示二维空间坐标的点，向量, 也可进行二维的数值运算 */
	template<typename T>
	class Point2 
	{
	public:
		Point2(void) {
			_v[0] = 0;
			_v[1] = 0;
		};

		// 拷贝构造函数
		Point2(const Point2& obj)
		{
			_v[0] = obj._v[0];
			_v[1] = obj._v[1];
		}

		// 方便用户的构造函数
		Point2(T x, T y) {
			_v[0] = x;
			_v[1] = y;
		}

	public:
		T& x(void);
		T& y(void);

		const T& x(void) const;
		const T& y(void) const;

		// 拷贝复制构造函数
		Point2& operator=(const Point2& point);

		///* 二维坐标常用操作*/
		bool operator==(const Point2& point) const;
		bool operator!=(const Point2& point) const;
		bool operator<(const Point2& point) const;

		Point2 operator+(const Point2& rhs) const;
		Point2 operator-(const Point2& rhs) const;
		Point2 operator*(T rhs) const;
		Point2 operator/(T rhs) const;

		const Point2& operator+=(const Point2& rhs);
		const Point2& operator-=(const Point2& rhs);
		const Point2& operator*=(const T& rhs);
		const Point2& operator/=(const T& rhs);

	public:
		void Set(T x, T y);
		T Length() const;
		Point2 Normalize();

	public:
		T _v[2];
	};

	typedef Point2<int>		Point2i;
	typedef Point2<float>	Point2f;
	typedef Point2<double>	Point2d;
}

#endif
