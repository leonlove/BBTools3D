#include "MathUtil.h"
#include "Types.h"
using namespace BBGLOBE;

int Math::EncodeNumber(int num)
{
	//return ((((num + 2) * 3) + 5) * 7) ^ 11; //点太多  int溢出
	return num ^ 11;
}

int Math::DecodeNumber(int num)
{
	//return (((num ^ 11) / 7) - 5) / 3 - 2;
	return num ^ 11;
}

//glm::dmat4 Math::TransformXYZ(double radian_x, double radian_y, double height_min)
//{
//	double offsetX = 0.0;
//	double offsetY = 0.0;
//	double offsetZ = 0.0;
//	Math::ConvertLatLongHeightToXYZ(radian_y, radian_x, height_min, offsetX, offsetY, offsetZ);
//
//	glm::dmat4 matrix = {
//		1,		0,		0,		0,
//		0,		1,		0,		0,
//		0,		0,		1,		0,
//		offsetX,offsetY,offsetZ,1
//	};
//	return matrix;
//}

glm::dmat3 Math::RotatedXYZ(double radian_x, double radian_y, double height_min)
{
	long double ellipsod_a = 40680631590769;
	long double ellipsod_b = 40680631590769;
	long double ellipsod_c = 40408299984661.4;

	long double xn = std::cos(radian_x) * std::cos(radian_y);
	long double yn = std::sin(radian_x) * std::cos(radian_y);
	long double zn = std::sin(radian_y);

	long double x0 = ellipsod_a * xn;
	long double y0 = ellipsod_b * yn;
	long double z0 = ellipsod_c * zn;

	std::vector<long double> east_mat = { -y0,x0,0 };
	std::vector<long double> north_mat = {
		(y0 * east_mat[2] - east_mat[1] * z0),
		(z0 * east_mat[0] - east_mat[2] * x0),
		(x0 * east_mat[1] - east_mat[0] * y0)
	};
	long double east_normal = std::sqrt(
		east_mat[0] * east_mat[0] +
		east_mat[1] * east_mat[1] +
		east_mat[2] * east_mat[2]
	);
	long double north_normal = std::sqrt(
		north_mat[0] * north_mat[0] +
		north_mat[1] * north_mat[1] +
		north_mat[2] * north_mat[2]
	);

	glm::dmat3 matrix = {
		east_mat[0] / east_normal,
		east_mat[1] / east_normal,
		east_mat[2] / east_normal,
		north_mat[0] / north_normal,
		north_mat[1] / north_normal,
		north_mat[2] / north_normal,
		xn,
		yn,
		zn
	};
	return matrix;
}

//
//glm::dmat4 Math::TransformXYZ_NoProj(double radian_x, double radian_y, double height_min)
//{
//	// 导向到Math::TransformXYZ 版本
//	return Math::TransformXYZ(radian_x, radian_y, height_min);
//
//	long double ellipsod_a = 40680631590769;
//	long double ellipsod_b = 40680631590769;
//	long double ellipsod_c = 40408299984661.4;
//
//	long double xn = std::cos(radian_x) * std::cos(radian_y);
//	long double yn = std::sin(radian_x) * std::cos(radian_y);
//	long double zn = std::sin(radian_y);
//
//	long double x0 = ellipsod_a * xn;
//	long double y0 = ellipsod_b * yn;
//	long double z0 = ellipsod_c * zn;
//	long double gamma = std::sqrt(xn * x0 + yn * y0 + zn * z0);
//	long double px = x0 / gamma;
//	long double py = y0 / gamma;
//	long double pz = z0 / gamma;
//
//	long double dx = xn * height_min;
//	long double dy = yn * height_min;
//	long double dz = zn * height_min;
//
//	std::vector<long double> east_mat = { -y0,x0,0 };
//	std::vector<long double> north_mat = {
//		(y0 * east_mat[2] - east_mat[1] * z0),
//		(z0 * east_mat[0] - east_mat[2] * x0),
//		(x0 * east_mat[1] - east_mat[0] * y0)
//	};
//	long double east_normal = std::sqrt(
//		east_mat[0] * east_mat[0] +
//		east_mat[1] * east_mat[1] +
//		east_mat[2] * east_mat[2]
//	);
//	long double north_normal = std::sqrt(
//		north_mat[0] * north_mat[0] +
//		north_mat[1] * north_mat[1] +
//		north_mat[2] * north_mat[2]
//	);
//
//	glm::dmat4 matrix = {
//		east_mat[0] / east_normal,
//		east_mat[1] / east_normal,
//		east_mat[2] / east_normal,
//		0,
//		north_mat[0] / north_normal,
//		north_mat[1] / north_normal,
//		north_mat[2] / north_normal,
//		0,
//		xn,
//		yn,
//		zn,
//		0,
//		px + dx,
//		py + dy,
//		pz + dz,
//		1
//	};
//	return matrix;
//}

 double Math::AutoCalculateGeometricError(unsigned int level, double screenSpaceError)
{
	// 默认与cesium 保持一致
	static double EarthRadius = 6371393.0;
	static double pi = std::acos(-1);
	static double zerrorError = EarthRadius * pi / 2 / screenSpaceError;
	unsigned int base = (1 << level);
	return zerrorError / base;
}

 bool Math::floatEqual(float flt1, float flt2, float fltEpsilon)
{
	const float fltD = fabs(flt1 - flt2);
	return (fltD <= fltEpsilon);
}

 bool Math::floatEqual(double flt1, double flt2, double fltEpsilon)
{
	const double fltD = fabs(flt1 - flt2);
	return (fltD <= fltEpsilon);
}

 double Math::Degree2Rad(double val)
{
	return val * PI / 180.0;
}

 double Math::Rad2Degree(double val)
{
	return val * 180.0 / PI;
}

 double Math::Lat2Meter(double diff)
{
	return diff / 0.000000157891;
}

 double Math::Long2Meter(double diff, double lati)
{
	return diff / 0.000000156785 * std::cos(lati);
}

 double Math::Meter2Lat(double m)
{
	return m * 0.000000157891;
}

 double Math::Meter2Long(double m, double lati)
{
	return m * 0.000000156785 / std::cos(lati);
}

 double Math::TriangleArea(double a, double b, double c)
 {
	 double p = (a + c + b) / 2.0;
	 double area = std::sqrt(p * (p - a) * (p - c) * (p - b));
	 if (isnan(area)) {
		 area = 0.0;
	 }
	 return area;
 }
double Math::TriangleArea(const glm::dvec3& a, const glm::dvec3& b, const glm::dvec3& c)
{
	glm::dvec3 vec_ab = a - b;
	glm::dvec3 vec_ac = a - c;
	glm::dvec3 vec_bc = b - c;

	double ab = glm::length(vec_ab);
	double ac = glm::length(vec_ac);
	double bc = glm::length(vec_bc);
	return TriangleArea(ab,bc,ac);
}

 bool Math::CheckPointOnTriangle(const glm::dvec3& A, const glm::dvec3& B, const glm::dvec3& C, const glm::dvec3& P)
{
	double area_ABC = Math::TriangleArea(A, B, C);
	double area_ABP = Math::TriangleArea(A, B, P);
	double area_ACP = Math::TriangleArea(A, C, P);
	double area_BCP = Math::TriangleArea(B, C, P);

	double thr = (area_ABP + area_ACP + area_BCP) / (area_ABC + 1e-7);

	if (thr > 1.0005) {
		return false;
	}

	return true;
}

 bool Math::Vec3Equal(const glm::dvec3& src, const glm::dvec3& dst, double eplison)
{
	if (std::fabs(src.x - dst.x) < eplison &&
		std::fabs(src.y - dst.y) < eplison &&
		std::fabs(src.z - dst.z) < eplison)
	{
		return true;
	}
	return false;
}

// void Math::ConvertLatLongHeightToXYZ(double latitude, double longitude, double height, double& X, double& Y, double& Z)
//{
//	const double  radiusEquator = 6378137.0;
//	const double  radiusPolar = 6356752.3142451793;
//
//	GeocentricInfo gi;
//	pj_Set_Geocentric_Parameters(&gi, radiusEquator, radiusPolar);
//	// 大地坐标转地心坐标
//	pj_Convert_Geodetic_To_Geocentric(&gi, latitude, longitude, height, &X, &Y, &Z);
//}

// void Math::ConvertXYZToLatLongHeight(double X, double Y, double Z, double& latitude, double& longitude, double& height)
//{
//	const double  radiusEquator = 6378137.0;
//	const double  radiusPolar = 6356752.3142451793;
//
//	GeocentricInfo gi;
//	pj_Set_Geocentric_Parameters(&gi, radiusEquator, radiusPolar);
//	//地心坐标转大地坐标
//	pj_Convert_Geocentric_To_Geodetic(&gi, X, Y, Z, &latitude, &longitude, &height);
//}

 void Math::ComputeCoordinateFrame(double latitude, double longitude, glm::dmat4& localToWorld)
{
	glm::dvec3 up;
	up.x = cos(longitude) * cos(latitude);
	up.y = sin(longitude) * cos(latitude);
	up.z = sin(latitude);

	glm::dvec3 east;
	east.x = -sin(longitude);
	east.y = cos(longitude);
	east.z = 0.0;

	glm::dvec3 north = glm::cross(up, east);

	localToWorld[0][0] = east.x;
	localToWorld[0][1] = east.y;
	localToWorld[0][2] = east.z;
	localToWorld[0][3] = 0.0;
	localToWorld[1][0] = north.x;
	localToWorld[1][1] = north.y;
	localToWorld[1][2] = north.z;
	localToWorld[1][3] = 0.0;
	localToWorld[2][0] = up.x;
	localToWorld[2][1] = up.y;
	localToWorld[2][2] = up.z;
	localToWorld[2][3] = 0.0;
}

// void Math::ComputeLocalToWorldTransformFromLatLongHeight(double latitude, double longitude, double height, glm::dmat4& localToWorld)
//{
//	double X, Y, Z;
//	Math::ConvertLatLongHeightToXYZ(latitude, longitude, height, X, Y, Z);
//
//	localToWorld[3][0] = X;
//	localToWorld[3][1] = Y;
//	localToWorld[3][2] = Z;
//	localToWorld[3][3] = 1.0;
//
//	Math::ComputeCoordinateFrame(latitude, longitude, localToWorld);
//}

 glm::dvec3 Math::TranslateAxis(const glm::dvec3& pt, double rx, double ry, double rz)
{
	glm::dvec3 vecX = glm::rotateX(pt, rx);
	glm::dvec3 vecY = glm::rotateY(vecX, ry);
	glm::dvec3 result = glm::rotateZ(vecY, rz);
	return result;
}

 glm::vec3 Math::TranslateAxis(const glm::vec3& pt, float rx, float ry, float rz)
{
	glm::vec3 vecX = glm::rotateX(pt, rx);
	glm::vec3 vecY = glm::rotateY(vecX, ry);
	glm::vec3 result = glm::rotateZ(vecY, rz);

	return result;
}

 bool Math::IsReversedFace(const glm::dmat4& mat)
{
	glm::dmat4 LocalMatrix(mat);

	LocalMatrix[3] = glm::dvec4(0, 0, 0, LocalMatrix[3].w);
	glm::dvec3 Row[3], Pdum3;

	for (size_t i = 0; i < 3; ++i) {
		for (size_t j = 0; j < 3; ++j) {
			Row[i][j] = LocalMatrix[i][j];
		}
	}

	// At this point, the matrix (in rows[]) is orthonormal.
	// Check for a coordinate system flip. If the determinant
	// is -1, then negate the matrix and the scaling factors.

	Pdum3 = glm::cross(Row[1], Row[2]);
	if (glm::dot(Row[0], Pdum3) < 0)
	{
		return true;
	}

	return false;
}

 double Math::ComputerGeometricError(double dMaxDis)
{
	return (0.4537010865381809 * dMaxDis / 50.0);
}

 int Math::ComputerLeveFromGeometriError(double geometricError)
{
	int num = 1;
	int level = 0;
	while (num < geometricError)
	{
		num <<= 1;
		level++;
	}
	return (20 - level);
}

double Math::RangeRandom(double min, double max)
{
	static std::default_random_engine dre;
	std::uniform_real_distribution<double> dr(min, max);
	return dr(dre);
}

glm::dvec2 Math::LonLat2Mercator(double longitude, double latitude)
{
	glm::dvec2 mercator;
	double x = longitude * 20037508.34 / 180.0;
	double y = std::log(std::tan((90 + latitude) * PI / 360.0)) / (PI / 180.0);
	y = y * 20037508.34 / 180.0;
	mercator.x = x;
	mercator.y = y;
	return mercator;
}

//double Math::DistanceBetweenLonLatHeight(double startLon, double startLat, double startHeight, double endLon, double endLat, double endHeight)
//{
//	glm::dvec3 startPos;
//	Math::ConvertLatLongHeightToXYZ(Math::Degree2Rad(startLat), Math::Degree2Rad(startLon), startHeight, startPos.x, startPos.y, startPos.z);
//	glm::dvec3 endPos;
//	Math::ConvertLatLongHeightToXYZ(Math::Degree2Rad(endLat), Math::Degree2Rad(endLon), endHeight, endPos.x, endPos.y, endPos.z);
//	return glm::length(endPos - startPos);
//}

double Math::Lerp(double p, double q, double time) 
{
	return ((1.0 - time) * p) + (time * q);
}

bool Math::IsInChina(double lon, double lat)
{
	return lon > 73 && lon < 135.1 && lat > 3.8 && lat < 53.6;
}
