#include "Transformation.h"
#include "FileSystemUtil.h"
#include "MathUtil.h"
#include "Log.h"
#include "StringUtil.h"

#include "ogr_spatialref.h"
#include "ogrsf_frmts.h"

using namespace BBGLOBE;

// 仿射变换的逆矩阵
//  | R , T |    | R^,-R^T|
//  | 0 , 1 |    | 0 , 1  |
static inline TransformMatrix Inverse(const TransformMatrix& matrix)
{
	TransformMatrix ret;
	ret.Data.resize(16);
	ret.Data[0] = matrix.Data[0];
	ret.Data[1] = matrix.Data[4];
	ret.Data[2] = matrix.Data[8];
	ret.Data[3] = matrix.Data[3];
	ret.Data[4] = matrix.Data[1];
	ret.Data[5] = matrix.Data[5];
	ret.Data[6] = matrix.Data[9];
	ret.Data[7] = matrix.Data[7];
	ret.Data[8] = matrix.Data[2];
	ret.Data[9] = matrix.Data[6];
	ret.Data[10] = matrix.Data[10];
	ret.Data[11] = matrix.Data[11];
	ret.Data[12] -= matrix.Data[12] * ret.Data[0] + matrix.Data[13] * ret.Data[4] + matrix.Data[14] * ret.Data[8];
	ret.Data[13] -= matrix.Data[12] * ret.Data[1] + matrix.Data[13] * ret.Data[5] + matrix.Data[14] * ret.Data[9];
	ret.Data[14] -= matrix.Data[12] * ret.Data[2] + matrix.Data[13] * ret.Data[6] + matrix.Data[14] * ret.Data[10];
	ret.Data[15] = matrix.Data[15];
	return ret;
}

std::mutex initGDALMutex;
bool gdalInited = false;
static inline void InitGDAL()
{
	std::lock_guard<std::mutex> lockGuard(initGDALMutex);

	if (gdalInited ) {
		return;
	}

	GDALAllRegister();
#ifdef  _WIN32
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");
#endif //  _WIN32

	std::string gdal = CPLGetConfigOption("GDAL_DATA", "");
	if (gdal == "")
	{
		std::string gdalDir = FileSystem::GetCurrentModuleDirectory();
		gdalDir = FileSystem::CombinePath(gdalDir, "gdaldata");
		CPLSetConfigOption("GDAL_DATA", gdalDir.c_str());
	}
	gdalInited = true;
}

Transformation::Transformation()
{
}

Transformation::~Transformation()
{
}

void Transformation::Transform(double& x, double& y, double& z)
{
	return Transform(&x, &y, &z, 1,3);
}

bool Transformation::IsValid()
{
	return m_ptrOGRSpatialReference != nullptr;
}

TransformMatrix::TransformMatrix()
{
}

TransformMatrix::~TransformMatrix()
{
}

TransformMatrix::TransformMatrix(const TransformMatrix& matrix)
{
	Data = matrix.Data;
}

TransformMatrix& TransformMatrix::operator=(const TransformMatrix& matrix)
{
	Data = matrix.Data;
	return *this;
}

void TransformMatrix::Multiply(double& x, double& y, double& z)
{
	double X = Data[0] * x + Data[4] * y + Data[8] * z + Data[12];
	double Y = Data[1] * x + Data[5] * y + Data[9] * z + Data[13];
	double Z = Data[2] * x + Data[6] * y + Data[10] * z + Data[14];
	double W = Data[3] * x + Data[7] * y + Data[11] * z + Data[15];
	x = X / W;
	y = Y / W;
	z = Z / W;
}


TransformMatrix& TransformMatrix::Multiply(const TransformMatrix& matrix)
{
	if (!matrix.IsValid())
		return *this;
	if (!IsValid())
	{
		*this = matrix;
		return *this;
	}

	const double* left = Data.data();
	double left0 = left[0];
	double left1 = left[1];
	double left2 = left[2];
	double left4 = left[4];
	double left5 = left[5];
	double left6 = left[6];
	double left8 = left[8];
	double left9 = left[9];
	double left10 = left[10];
	double left12 = left[12];
	double left13 = left[13];
	double left14 = left[14];

	const double* right = matrix.Data.data();

	double right0 = right[0];
	double right1 = right[1];
	double right2 = right[2];
	double right4 = right[4];
	double right5 = right[5];
	double right6 = right[6];
	double right8 = right[8];
	double right9 = right[9];
	double right10 = right[10];
	double right12 = right[12];
	double right13 = right[13];
	double right14 = right[14];

	double column0Row0 = left0 * right0 + left4 * right1 + left8 * right2;
	double column0Row1 = left1 * right0 + left5 * right1 + left9 * right2;
	double column0Row2 = left2 * right0 + left6 * right1 + left10 * right2;

	double column1Row0 = left0 * right4 + left4 * right5 + left8 * right6;
	double column1Row1 = left1 * right4 + left5 * right5 + left9 * right6;
	double column1Row2 = left2 * right4 + left6 * right5 + left10 * right6;

	double column2Row0 = left0 * right8 + left4 * right9 + left8 * right10;
	double column2Row1 = left1 * right8 + left5 * right9 + left9 * right10;
	double column2Row2 = left2 * right8 + left6 * right9 + left10 * right10;

	double column3Row0 = left0 * right12 + left4 * right13 + left8 * right14 + left12;
	double column3Row1 = left1 * right12 + left5 * right13 + left9 * right14 + left13;
	double column3Row2 = left2 * right12 + left6 * right13 + left10 * right14 + left14;

	double* result = this->Data.data();

	result[0] = column0Row0;
	result[1] = column0Row1;
	result[2] = column0Row2;
	result[3] = 0.0;
	result[4] = column1Row0;
	result[5] = column1Row1;
	result[6] = column1Row2;
	result[7] = 0.0;
	result[8] = column2Row0;
	result[9] = column2Row1;
	result[10] = column2Row2;
	result[11] = 0.0;
	result[12] = column3Row0;
	result[13] = column3Row1;
	result[14] = column3Row2;
	result[15] = 1.0;
	return *this;
}

bool TransformMatrix::IsValid() const
{
	return Data.size() == 16;
}

TransformMatrix::operator bool() const
{
	return IsValid();
}

bool TransformMatrix::IsIdentity()
{
	std::vector<double> data = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
	for (size_t i = 0; i < data.size(); i++)
	{
		if (std::abs(Data[i] - data[i]) > 1e-8)
			return false;
	}
	return true;
}

ENU2GeocentricTransformation::ENU2GeocentricTransformation(double degreeL, double degreeB,  double h, int srEPSG)
	:Transformation()
{
	InitGDAL();
	double radianB = Math::ToRadian(degreeB);
	double radianL = Math::ToRadian(degreeL);

	m_ptrOGRSpatialReference = std::make_shared<OGRSpatialReference>();
	m_ptrOGRSpatialReference->SetAxisMappingStrategy(OSRAxisMappingStrategy::OAMS_TRADITIONAL_GIS_ORDER);
	//auto error = m_ptrOGRSpatialReference->importFromEPSG(srEPSG);

	mTransformMatrix.Data.resize(16);
	double a = m_ptrOGRSpatialReference->GetSemiMajor();
	double f = 1.0 / m_ptrOGRSpatialReference->GetInvFlattening();
	///旋转矩阵9元素
	mTransformMatrix.Data[0] = -sin(radianL);
	mTransformMatrix.Data[1] = cos(radianL);
	mTransformMatrix.Data[2] = 0;
	mTransformMatrix.Data[3] = 0;
	mTransformMatrix.Data[4] = -sin(radianB) * cos(radianL);
	mTransformMatrix.Data[5] = -sin(radianB) * sin(radianL);
	mTransformMatrix.Data[6] = cos(radianB);
	mTransformMatrix.Data[7] = 0;

	mTransformMatrix.Data[8] = cos(radianB) * cos(radianL);
	mTransformMatrix.Data[9] = cos(radianB) * sin(radianL);
	mTransformMatrix.Data[10] = sin(radianB);
	mTransformMatrix.Data[11] = 0;
	///椭球第一偏心率
	double e2 = 2.0 * f - f * f;
	double N = a / sqrt(1 - e2 * sin(radianB) * sin(radianB));
	///地心原点到站心原点的平移参数
	mTransformMatrix.Data[12] = (N + h) * cos(radianB) * cos(radianL);
	mTransformMatrix.Data[13] = (N + h) * cos(radianB) * sin(radianL);
	mTransformMatrix.Data[14] = (N * (1 - e2) + h) * sin(radianB);
	mTransformMatrix.Data[15] = 1.0;
}

ENU2GeocentricTransformation::~ENU2GeocentricTransformation()
{
}

void ENU2GeocentricTransformation::Transform(double* pX, double* pY, double* pZ, int nCount, int offset)
{
	for (int i = 0; i < nCount; ++i)
	{
		mTransformMatrix.Multiply(pX[i * offset], pY[i * offset], pZ[i * offset]);
	}
}

bool ENU2GeocentricTransformation::IsValid()
{
	return mTransformMatrix.IsValid();
}

TransformMatrix ENU2GeocentricTransformation::GetTransformMatrix() const
{
	return mTransformMatrix;
}

Geocentric2GeoTransformation::Geocentric2GeoTransformation(int EPSG) :Transformation()
{
	InitGDAL();
	m_ptrOGRSpatialReference = std::make_shared<OGRSpatialReference>();
	m_ptrOGRSpatialReference->SetAxisMappingStrategy(OSRAxisMappingStrategy::OAMS_TRADITIONAL_GIS_ORDER);
	m_ptrOGRSpatialReference->importFromEPSG(EPSG);
}

Geocentric2GeoTransformation::~Geocentric2GeoTransformation()
{
}

void Geocentric2GeoTransformation::Transform(double* pX, double* pY, double* pZ, int nCount, int offset)
{
	static double pi = std::acos(-1);
	double a = m_ptrOGRSpatialReference->GetSemiMajor();
	double f = 1.0 / m_ptrOGRSpatialReference->GetInvFlattening();

	static const double  genau = 1.E-12;
	static const double genau2 = (genau * genau);
	static int maxiter = 30;

	double e2 = (2 * f) - (f * f);
	//double b = a * sqrt(1 - e2);

	for (int i = 0; i < nCount; i++)
	{
		double X, Y, Z;
		double H = 0;
		double* Longitude = pX + offset * i;
		double* Latitude = pY + offset * i;
		double* Height = &H;
		if (pZ) Height = pZ + offset * i;
		X = *Longitude;
		Y = *Latitude;
		Z = *Height;

		//使用proj4算法，迭代次数更少
		double P, RR, CT, ST, RX, RK, RN, CB0, SB0, CB1, SB1, SDPHI;
		int iter;

		P = sqrt(X * X + Y * Y);
		RR = sqrt(X * X + Y * Y + Z * Z);
		CT = Z / RR;
		ST = P / RR;

		if (P / a < genau)
		{
			*Longitude = 0.;
			if (RR / a < genau)
			{
				*Latitude = 90.;
				*Height = f * a - a;
				continue;
			}
		}
		else
			*Longitude = atan2(Y, X) / pi * 180.;

		const double denominator = 1.0 - e2 * (2.0 - e2) * ST * ST;
		if (denominator == 0)
			return;
		RX = 1.0 / sqrt(denominator);
		CB0 = ST * (1.0 - e2) * RX;
		SB0 = CT * RX;
		iter = 0;
		bool IsLatitudeZero = false;
		do
		{
			++iter;
			RN = a / sqrt(1.0 - e2 * SB0 * SB0);
			*Height = P * CB0 + Z * SB0 - RN * (1.0 - e2 * SB0 * SB0);
			if (Math::IsFloatEqual(RN + *Height, 0.0))
			{
				IsLatitudeZero = true;
				*Latitude = 0.0;
				break;
			}
			RK = e2 * RN / (RN + *Height);

			const double denominator = 1.0 - RK * (2.0 - RK) * ST * ST;
			if (denominator == 0)
				return ;
			RX = 1.0 / sqrt(denominator);

			CB1 = ST * (1.0 - RK) * RX;
			SB1 = CT * RX;
			SDPHI = SB1 * CB0 - CB1 * SB0;
			CB0 = CB1;
			SB0 = SB1;
		} while (SDPHI * SDPHI > genau2 && iter < maxiter);
		if (!IsLatitudeZero)
			*Latitude = atan2(SB1, fabs(CB1)) / pi * 180;
	}
}

Project2GeoTransformation::Project2GeoTransformation(int EPSG, int targetEPSG)
	:Transformation()
{
	InitGDAL();
	m_ptrOGRSpatialReference = std::make_shared<OGRSpatialReference>();
	m_ptrOGRSpatialReference->SetAxisMappingStrategy(OSRAxisMappingStrategy::OAMS_TRADITIONAL_GIS_ORDER);
	m_ptrOGRSpatialReference->importFromEPSG(targetEPSG);
	m_ptrOGRSpatialReference->SetAxisMappingStrategy(OSRAxisMappingStrategy::OAMS_TRADITIONAL_GIS_ORDER);

	m_ptrSrcOGRSpatialReference = std::make_shared<OGRSpatialReference>();
	m_ptrSrcOGRSpatialReference->SetAxisMappingStrategy(OSRAxisMappingStrategy::OAMS_TRADITIONAL_GIS_ORDER);
	m_ptrSrcOGRSpatialReference->importFromEPSG(EPSG);
	m_ptrSrcOGRSpatialReference->SetAxisMappingStrategy(OSRAxisMappingStrategy::OAMS_TRADITIONAL_GIS_ORDER);

	m_pOGRTransformation = OGRCreateCoordinateTransformation(m_ptrSrcOGRSpatialReference.get(), m_ptrOGRSpatialReference.get());

}

Project2GeoTransformation::Project2GeoTransformation(const std::string& wkt, int targetEPSG)
	:Transformation()
{
	InitGDAL();
	m_ptrOGRSpatialReference = std::make_shared<OGRSpatialReference>();
	m_ptrOGRSpatialReference->SetAxisMappingStrategy(OSRAxisMappingStrategy::OAMS_TRADITIONAL_GIS_ORDER);
	m_ptrOGRSpatialReference->importFromEPSG(targetEPSG);
	m_ptrOGRSpatialReference->SetAxisMappingStrategy(OSRAxisMappingStrategy::OAMS_TRADITIONAL_GIS_ORDER);

	m_ptrSrcOGRSpatialReference = std::make_shared<OGRSpatialReference>();
	m_ptrSrcOGRSpatialReference->SetAxisMappingStrategy(OSRAxisMappingStrategy::OAMS_TRADITIONAL_GIS_ORDER);
	m_ptrSrcOGRSpatialReference->importFromProj4(wkt.c_str());
	m_ptrSrcOGRSpatialReference->SetAxisMappingStrategy(OSRAxisMappingStrategy::OAMS_TRADITIONAL_GIS_ORDER);

	m_pOGRTransformation = OGRCreateCoordinateTransformation(m_ptrSrcOGRSpatialReference.get(), m_ptrOGRSpatialReference.get());

}

Project2GeoTransformation::~Project2GeoTransformation()
{
	if (m_pOGRTransformation != nullptr)
		OGRCoordinateTransformation::DestroyCT(m_pOGRTransformation);
	m_pOGRTransformation = nullptr;
}

void Project2GeoTransformation::Transform(double* pX, double* pY, double* pZ, int nCount, int offset)
{
	std::lock_guard<std::mutex> lock(mMutex);
	for (int i = 0; i < nCount; ++i)
	{
		m_pOGRTransformation->Transform(1, pX + i * offset, pY + i * offset, pZ + i * offset);
	}
}

bool Project2GeoTransformation::IsValid()
{
	return m_pOGRTransformation != nullptr;
}

Geo2GeocentricTransformation::Geo2GeocentricTransformation(int EPSG)
{
	InitGDAL();
	m_ptrOGRSpatialReference = std::make_shared<OGRSpatialReference>();
	m_ptrOGRSpatialReference->SetAxisMappingStrategy(OSRAxisMappingStrategy::OAMS_TRADITIONAL_GIS_ORDER);
	m_ptrOGRSpatialReference->importFromEPSG(EPSG);
}

Geo2GeocentricTransformation::~Geo2GeocentricTransformation()
{
}


void Geo2GeocentricTransformation::Transform(double* pX, double* pY, double* pZ, int nCount, int offset)
{
	double a = m_ptrOGRSpatialReference->GetSemiMajor();
	double f = 1 / m_ptrOGRSpatialReference->GetInvFlattening();
	double e2 = 2.0 * f - f * f;

	double radianB = 0, radianL = 0, H = 0;
	for (int i = 0; i < nCount; ++i)
	{
		radianL = Math::ToRadian(pX[i * offset]);
		radianB = Math::ToRadian(pY[i * offset]);
		if (pZ != nullptr)
			H = pZ[i * offset];
		double N = a / sqrt(1 - e2 * sin(radianB) * sin(radianB));
		pX[i * offset] = (N + H) * cos(radianB) * cos(radianL);
		pY[i * offset] = (N + H) * cos(radianB) * sin(radianL);
		if (pZ != nullptr)
			pZ[i * offset] = (N * (1 - e2) + H) * sin(radianB);
	}
}

bool Geo2GeocentricTransformation::IsValid()
{
	return m_ptrOGRSpatialReference != nullptr;
}

Geocentric2ENUTransformation::Geocentric2ENUTransformation(double degreeL, double degreeB,  double h, int srEPSG)
{
	mTransformMatrix = ModelCoordinateSystem::ENUMatrix(degreeL, degreeB, h);
	mTransformMatrix = Inverse(mTransformMatrix);
}

Geocentric2ENUTransformation::~Geocentric2ENUTransformation()
{
}

bool Geocentric2ENUTransformation::IsValid()
{
	return mTransformMatrix.IsValid();
}

void Geocentric2ENUTransformation::Transform(double* pX, double* pY, double* pZ, int nCount, int offset)
{
	for (int i = 0; i < nCount; ++i)
	{
		mTransformMatrix.Multiply(pX[i * offset], pY[i * offset], pZ[i * offset]);
	}
}

TransformMatrix Geocentric2ENUTransformation::GetTransformMatrix() const
{
	return mTransformMatrix;
}

Geo2ProjectTransformation::Geo2ProjectTransformation(int tarEPSG, int srcEPSG)
{
	InitGDAL();
	m_ptrOGRSpatialReference = std::make_shared<OGRSpatialReference>();
	m_ptrOGRSpatialReference->SetAxisMappingStrategy(OSRAxisMappingStrategy::OAMS_TRADITIONAL_GIS_ORDER);
	m_ptrOGRSpatialReference->importFromEPSG(tarEPSG);
	m_ptrOGRSpatialReference->SetAxisMappingStrategy(OSRAxisMappingStrategy::OAMS_TRADITIONAL_GIS_ORDER);

	m_ptrSrcOGRSpatialReference = std::make_shared<OGRSpatialReference>();
	m_ptrSrcOGRSpatialReference->SetAxisMappingStrategy(OSRAxisMappingStrategy::OAMS_TRADITIONAL_GIS_ORDER);
	m_ptrSrcOGRSpatialReference->importFromEPSG(srcEPSG);
	m_ptrSrcOGRSpatialReference->SetAxisMappingStrategy(OSRAxisMappingStrategy::OAMS_TRADITIONAL_GIS_ORDER);

	m_pOGRTransformation = OGRCreateCoordinateTransformation(m_ptrSrcOGRSpatialReference.get(), m_ptrOGRSpatialReference.get());

}

Geo2ProjectTransformation::~Geo2ProjectTransformation()
{
}

void Geo2ProjectTransformation::Transform(double* pX, double* pY, double* pZ, int nCount, int offset)
{
	std::lock_guard<std::mutex> lock(mMutex);
	for (int i = 0; i < nCount; ++i)
	{
		m_pOGRTransformation->Transform(1, pX + i * offset, pY + i * offset, pZ + i * offset);
	}
}
bool Geo2ProjectTransformation::IsValid()
{
	return m_pOGRTransformation != nullptr;
}


//using namespace irr::io;

ModelCoordinateSystem::ModelCoordinateSystem(const std::string& metafile)
	:m_CoordinateSystemType(CoordinateSystemType::eENU), mMetaDataFile(metafile)
{
	Init(metafile);
}

ModelCoordinateSystem::ModelCoordinateSystem(const Point3d& center)
	: m_Origin(center), m_CoordinateSystemType(CoordinateSystemType::eENU)
{
	m_ptrENU2GeocentricTransformation = std::make_shared<ENU2GeocentricTransformation>(m_Origin.x(), m_Origin.y(), m_Origin.z());
	m_ptrGeocentric2ENUTransformation = std::make_shared<Geocentric2ENUTransformation>(m_Origin.x(), m_Origin.y(), m_Origin.z());

	m_ptrGeo2GeocentricTransformation = std::make_shared<Geo2GeocentricTransformation>();
	m_ptrGeocentric2GeoTransformation = std::make_shared<Geocentric2GeoTransformation>();
}

ModelCoordinateSystem::ModelCoordinateSystem(double degreeL, double degreeB, double height)
	:ModelCoordinateSystem(Point3d(degreeL, degreeB, height)) 
{
}

ModelCoordinateSystem::~ModelCoordinateSystem()
{
}

TransformMatrix ModelCoordinateSystem::GetTransformMatirx()
{
	if (!m_Transform.IsValid())
	{
		ENU2GeocentricTransformation* pEnuCS = dynamic_cast<ENU2GeocentricTransformation*>(m_ptrENU2GeocentricTransformation.get());
		m_Transform = pEnuCS->GetTransformMatrix();
	}
	return m_Transform;
}

Point3<double> ModelCoordinateSystem::GetOrigin()
{
	return m_Origin;
}

void ModelCoordinateSystem::Init(const std::string& metadataFile)
{
	if (!FileSystem::FileExists(metadataFile))
		return;

	std::string sSrs = "";
	std::string sSrsOri = "";

	//if (!GetMetaData(metadataFile, sSrs, sSrsOri))
	//	return;

	std::vector<std::string> srsLst = StringUtil::Split(sSrs, ":");
	if (srsLst.size() != 2)
		return;
	std::string sPrjType = srsLst[0];
	if (sPrjType == "ENU" || sPrjType == "enu")
	{
		m_CoordinateSystemType = CoordinateSystemType::eENU;
		std::vector<std::string> coorlst = StringUtil::Split(srsLst[1], ",");
		if (coorlst.size() < 2)
		{
			LogIns.error("parse ENU point error");
			return;
		}
		m_Origin.x() = atof(coorlst[1].c_str());
		m_Origin.y() = atof(coorlst[0].c_str());
		if (coorlst.size() > 2)
			m_Origin.z() = atof(coorlst[2].c_str());
		m_ptrENU2GeocentricTransformation = std::make_shared<ENU2GeocentricTransformation>(m_Origin.x(), m_Origin.y(), m_Origin.z());
		m_ptrGeocentric2ENUTransformation = std::make_shared<Geocentric2ENUTransformation>(m_Origin.x(), m_Origin.y(), m_Origin.z());
	}
	else if (sPrjType == "EPSG" || sPrjType == "epsg") {
		m_CoordinateSystemType = CoordinateSystemType::eProject;
		int isrs = atoi(srsLst[1].c_str());
		m_ptrProject2GeoTransformation = std::make_shared<Project2GeoTransformation>(isrs);
		m_ptrGeo2ProjectTransformation = std::make_shared<Geo2ProjectTransformation>(isrs);
		std::vector<std::string> coorlst = StringUtil::Split(sSrsOri, ",");
		if (coorlst.size() > 2)
		{
			m_Location.x() = atof(coorlst[0].c_str());
			m_Location.y() = atof(coorlst[1].c_str());
			m_Location.z() = atof(coorlst[2].c_str());
			m_Origin = m_Location;
			m_ptrProject2GeoTransformation->Transform(m_Origin.x(), m_Origin.y(), m_Origin.z());
			m_ptrENU2GeocentricTransformation = std::make_shared<ENU2GeocentricTransformation>(m_Origin.x(), m_Origin.y(), m_Origin.z());
		}
		else
		{
			LogIns.error("parse epsg point error");
		}
	}
	else
	{
		LogIns.error("Error: fail to get metadata infomation");
	}
	m_ptrGeo2GeocentricTransformation = std::make_shared<Geo2GeocentricTransformation>();
	m_ptrGeocentric2GeoTransformation = std::make_shared<Geocentric2GeoTransformation>();
}

void ModelCoordinateSystem::Model2Geo(double* pX, double* pY, double* pZ, size_t num, size_t offset)
{
	if (m_CoordinateSystemType == CoordinateSystemType::eENU)
	{
		m_ptrENU2GeocentricTransformation->Transform(pX, pY, pZ, num, offset);
		m_ptrGeocentric2GeoTransformation->Transform(pX, pY, pZ, num, offset);
		return;
	}

	if (m_CoordinateSystemType == CoordinateSystemType::eProject)
	{
		for (size_t i = 0; i < num; ++i)
		{
			pX[i * offset] += m_Location.x();
			pY[i * offset] += m_Location.y();
			pZ[i * offset] += m_Location.z();
			m_ptrProject2GeoTransformation->Transform(pX[i * offset], pY[i * offset], pZ[i * offset]);
		}
	}
}

void ModelCoordinateSystem::Geo2Model(double* pX, double* pY, double* pZ, size_t num, size_t offset)
{
	if (m_CoordinateSystemType == CoordinateSystemType::eENU)
	{
		m_ptrGeo2GeocentricTransformation->Transform(pX, pY, pZ, num, offset);
		m_ptrGeocentric2ENUTransformation->Transform(pX, pY, pZ, num, offset);
		return;
	}

	if (m_CoordinateSystemType == CoordinateSystemType::eProject)
	{
		for (size_t i = 0; i < num; ++i)
		{
			m_ptrGeo2ProjectTransformation->Transform(pX[i * offset], pY[i * offset], pZ[i * offset]);
			pX[i * offset] -= m_Location.x();
			pY[i * offset] -= m_Location.y();
			pZ[i * offset] -= m_Location.z();
		}
	}
}

void ModelCoordinateSystem::Model2Geo(double& x, double& y, double& z)
{
	Model2Geo(&x, &y, &z, 1, 0);
}

void ModelCoordinateSystem::Geo2Model(double& x, double& y, double& z)
{
	Geo2Model(&x, &y, &z, 1, 0);
}

void ModelCoordinateSystem::Model2Geo(BoundingVolumeBox& box)
{
	if (!box.IsValid())
		return;
	double xmin = box.mData[0], ymin = box.mData[1], zmin = box.mData[4];
	double xmax = box.mData[2], ymax = box.mData[3], zmax = box.mData[5];
	Model2Geo(xmin,ymin,zmin);
	Model2Geo(xmax,ymax,zmax);
	box.mData[0] = std::min(xmin, xmax);
	box.mData[1] = std::min(ymin, ymax);
	box.mData[2] = std::max(xmin, xmax);
	box.mData[3] = std::max(ymin, ymax);
	box.mData[4] = std::min(zmin, zmax);
	box.mData[5] = std::max(zmin, zmax);
}

void ModelCoordinateSystem::Geo2Model(BoundingVolumeBox& box)
{
	if (!box.IsValid())
		return;
	double xmin = box.mData[0], ymin = box.mData[1], zmin = box.mData[4];
	double xmax = box.mData[2], ymax = box.mData[3], zmax = box.mData[5];
	Geo2Model(xmin, ymin, zmin);
	Geo2Model(xmax, ymax, zmax);
	box.mData[0] = std::min(xmin, xmax);
	box.mData[1] = std::min(ymin, ymax);
	box.mData[2] = std::max(xmin, xmax);
	box.mData[3] = std::max(ymin, ymax);
	box.mData[4] = std::min(zmin, zmax);
	box.mData[5] = std::max(zmin, zmax);
}


TransformMatrix ModelCoordinateSystem::ENUMatrix(const Point3d& pt)
{
	ENU2GeocentricTransformation trans(pt.x(), pt.y(), pt.z());
	return trans.GetTransformMatrix();
}

TransformMatrix ModelCoordinateSystem::ENUMatrix(double degreeL, double degreeB, double h)
{
	ENU2GeocentricTransformation trans(degreeL, degreeB, h);
	return trans.GetTransformMatrix();
}

TransformMatrix ModelCoordinateSystem::ENURelativeMatrix(const Point3d& from, const Point3d& to)
{
	auto fromMatrix = ENUMatrix(from);
	auto toMatrix = ENUMatrix(to);
	return Inverse(toMatrix).Multiply(fromMatrix);
}

TransformMatrix ModelCoordinateSystem::ENURelativeMatrix(double fromDegreeL, double fromDegreeB, double fromh, double toDegreeL, double toDegreeB, double toh)
{
	auto fromMatrix = ENUMatrix(fromDegreeL, fromDegreeB, fromh);
	auto toMatrix = ENUMatrix(toDegreeL, toDegreeB, toh);

	return Inverse(toMatrix).Multiply(fromMatrix);
}

TransformationPtr BBGLOBE::CreateTransformation(const std::string& proj)
{
	TransformationPtr ptrTrans = nullptr;
	if (!proj.empty())
	{
		int projEPSG = StringUtil::SafeStoi(proj);
		if (projEPSG > 0)
			ptrTrans = std::make_shared<Project2GeoTransformation>(projEPSG);
		else
			ptrTrans = std::make_shared<Project2GeoTransformation>(proj);
	}
	return ptrTrans;
}