#pragma once
#ifndef POINT2_INL_
#define POINT2_INL_

#include "Point2.h"
using namespace BBGLOBE;

template<typename T>
T& Point2<T>::x() {
	return _v[0];
}

template<typename T>
T& Point2<T>::y() {
	return _v[1];
}

template<typename T>
const T& Point2<T>::x(void) const { return _v[0]; }

template<typename T>
const T& Point2<T>::y(void) const { return _v[1]; }

template<typename T>
Point2<T>& Point2<T>::operator=(const Point2<T>& point) {
	if (this == &point) return *this;
	_v[0] = point._v[0];
	_v[1] = point._v[1];
	return *this;
}

template<typename T>
bool Point2<T>::operator==(const Point2<T>& point) const {
	return _v[0] == point._v[0] && _v[1] == point._v[1];
}

template<typename T>
bool Point2<T>::operator!=(const Point2<T>& point) const {
	return _v[0] != point._v[0] || _v[1] != point._v[1];
}

template<typename T>
bool Point2<T>::operator<(const Point2<T>& point) const {
	return _v[0] != point._v[0] ? _v[0] < point._v[0] : _v[1] < point._v[1];
}

template<typename T>
Point2<T> Point2<T>::operator+(const Point2<T>& rhs) const {
	return Point2<T>(_v[0] + rhs._v[0], _v[1] + rhs._v[1]);
}

template<typename T>
Point2<T> Point2<T>::operator-(const Point2<T>& rhs) const {
	return Point2<T>(_v[0] - rhs._v[0], _v[1] - rhs._v[1]);
}

template<typename T>
Point2<T> Point2<T>::operator*(T rhs) const {
	return Point2<T>(_v[0] * rhs, _v[1] * rhs);
}

template<typename T>
Point2<T> Point2<T>::operator/(T rhs) const {
	return Point2<T>(_v[0] / rhs, _v[1] / rhs);
}


template<typename T>
const Point2<T>& Point2<T>::operator+=(const Point2<T>& rhs) {
	_v[0] += rhs._v[0];
	_v[1] += rhs._v[1];
	return *this;
}

template<typename T>
const Point2<T>& Point2<T>::operator-=(const Point2<T>& rhs) {
	_v[0] -= rhs._v[0];
	_v[1] -= rhs._v[1];
	return *this;
}

template<typename T>
const Point2<T>& Point2<T>::operator*=(const T& rhs) {
	_v[0] *= rhs;
	_v[1] *= rhs;
	return *this;
}

template<typename T>
const Point2<T>& Point2<T>::operator/=(const T& rhs) {
	_v[0] /= rhs;
	_v[1] /= rhs;
	return *this;
}

template<typename T>
void Point2<T>::Set(T x, T y) {
	_v[0] = x;
	_v[1] = y;
}

template<typename T>
T Point2<T>::Length() const {
	return std::sqrt(_v[0] * _v[0] + _v[1] * _v[1]);
}

template<typename T>
Point2<T> Point2<T>::Normalize(){
	*this /= Length();
	return *this;
}
#endif
