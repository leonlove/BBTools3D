#pragma once

#include "stdafx.h"

#include <glm/matrix.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/ext/vector_relational.hpp>

namespace BBGLOBE
{

	class Math
	{
	public:
	template <typename T, typename A, typename B>
	static inline T Clamp(T src, A min, B max)
	{
		if (src < min) {
			return min;
		}
		if (src > max) {
			return max;
		}
		return src;
	}

	template <typename T, typename A>
	static inline T Max(T v1, A v2) {
		if (v1 > v2) {
			return v1;
		}
		return v2;
	}

	template <typename T, typename A>
	static inline T Min(T v1, A v2) {
		if (v1 < v2) {
			return v1;
		}
		return v2;
	}

	template <typename A, typename B, typename C, typename D>
	static inline A Clamp(B input, C min, D max) {
		if (input < min) {
			return min;
		}
		if (input > max) {
			return max;
		}
		return input;
	}

	template <typename T>
	static void Swap(T& a, T& b)
	{
		T tmp = a;
		a = b;
		b = tmp;
	}

	template<typename T>
	static bool IsFloatEqual(T a, T b)
	{
		if (a == b)
			return true;
		return fabs(a - b) < 4 * std::numeric_limits<T>::epsilon() * std::max(std::fabs(a), std::fabs(b));
	}

	template<typename T>
	static inline T ToRadian(T degree)
	{
		static T pi = std::acos(-1);
		return degree * pi / 180.0;
	}

	template<typename T>
	static inline T ToDegree(T radian)
	{
		static T pi = std::acos(-1);
		return radian * 180.0 / pi;
	}

	static int EncodeNumber(int num);

	static int DecodeNumber(int num);

	//static glm::dmat4 TransformXYZ(double radian_x, double radian_y, double height_min);

	static glm::dmat3 RotatedXYZ(double radian_x, double radian_y, double height_min);

	//static glm::dmat4 TransformXYZ_NoProj(double radian_x, double radian_y, double height_min);

	static double AutoCalculateGeometricError(unsigned int level, double screenSpaceError = 8.0);

	static bool floatEqual(float flt1, float flt2, float fltEpsilon = 1e-6f);

	static bool floatEqual(double flt1, double flt2, double fltEpsilon = 1e-9f);

	static double Degree2Rad(double val);

	static double Rad2Degree(double val);

	static double Lat2Meter(double diff);

	static double Long2Meter(double diff, double lati);

	static double Meter2Lat(double m);

	static double Meter2Long(double m, double lati);

	static double TriangleArea(const glm::dvec3& a, const glm::dvec3& b, const glm::dvec3& c);

	static double TriangleArea(double a, double b, double c);

	// 判断一个点是否在空间三角形上
	static bool CheckPointOnTriangle(const glm::dvec3& A, const glm::dvec3& B, const glm::dvec3& C, const glm::dvec3& P);

	static bool Vec3Equal(const glm::dvec3& src, const glm::dvec3& dst, double eplison = 0.0001);


	//static void ConvertLatLongHeightToXYZ(double latitude, double longitude, double height,
	//	double& X, double& Y, double& Z);

	//static void ConvertXYZToLatLongHeight(double X, double Y, double Z,
	//	double& latitude, double& longitude, double& height);


	static void ComputeCoordinateFrame(double latitude, double longitude, glm::dmat4& localToWorld);

	//static void ComputeLocalToWorldTransformFromLatLongHeight(double latitude, double longitude, double height, glm::dmat4& localToWorld);

	static glm::dvec3 TranslateAxis(const glm::dvec3& pt, double rx, double ry, double rz);

	static glm::vec3 TranslateAxis(const glm::vec3& pt, float rx, float ry, float rz);

	static bool IsReversedFace(const glm::dmat4& mat);

	static double ComputerGeometricError(double dMaxDis);

	static int ComputerLeveFromGeometriError(double geometricError);

	static double RangeRandom(double min, double max);

	static glm::dvec2 LonLat2Mercator(double longitude, double latitude);

	//static double DistanceBetweenLonLatHeight(double startLon, double startLat, double startHeight,
	//	double endLon, double endLat, double endHeight);

	static double Lerp(double p, double q, double time);

	static bool IsInChina(double lon, double lat);

	};

}
