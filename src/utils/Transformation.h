#pragma once

#include "stdafx.h"
#include "Point3.h"
#include "BoundingVolume.h"

class OGRCoordinateTransformation;
class OGRSpatialReference;
namespace BBGLOBE
{

/**
 * @brief 4X4模型转换矩阵，列主序
 */
class TransformMatrix
{
public:
	TransformMatrix();
	~TransformMatrix();
	TransformMatrix(const TransformMatrix& matrix);
	TransformMatrix& operator=(const TransformMatrix& matrix);

	std::vector<double> Data;

	void Multiply(double& x, double& y, double& z);

	TransformMatrix& Multiply(const TransformMatrix& matrix);

	bool IsValid() const;

	operator bool() const;

	bool IsIdentity();
};


/**
 * @brief 坐标转换.
 */
class Transformation
{
protected:
	std::shared_ptr<OGRSpatialReference> m_ptrOGRSpatialReference;
	Transformation();
	std::mutex mMutex;

public:
	virtual ~Transformation();
	virtual void Transform(double& x, double& y, double& z);
	virtual void Transform(double* pX, double* pY, double* pZ,int nCount,int offset) = 0;
	virtual bool IsValid();
};


/**
 * @brief 站心坐标系(ENU)到地心坐标系转换
 */
class ENU2GeocentricTransformation :public Transformation
{
private:
	TransformMatrix mTransformMatrix;
public:
	ENU2GeocentricTransformation(double degreeL,double degreeB, double h,int srEPSG = 4326);
	virtual ~ENU2GeocentricTransformation();

	virtual void Transform(double* pX, double* pY, double* pZ, int nCount,int offset);

	TransformMatrix GetTransformMatrix() const;

	virtual bool IsValid();
};

/**
 * @brief 地心坐标系到站心坐标系转换(ENU)
 */
class Geocentric2ENUTransformation :public Transformation
{
private:
	TransformMatrix mTransformMatrix;
public:
	Geocentric2ENUTransformation(double degreeL,double degreeB, double h, int srEPSG = 4326);
	virtual ~Geocentric2ENUTransformation();

	virtual void Transform(double* pX, double* pY, double* pZ, int nCount, int offset);

	TransformMatrix GetTransformMatrix() const;

	virtual bool IsValid();
};

/**
 * @brief 地心坐标系到地理坐标系转换
 */
class Geocentric2GeoTransformation :public Transformation
{
public:
	Geocentric2GeoTransformation(int EPSG = 4326);
	virtual ~Geocentric2GeoTransformation();
	virtual void Transform(double* pX, double* pY, double* pZ, int nCount, int offset);
};

/**
 * @brief 投影坐标系到地理坐标系转换
 */
class Project2GeoTransformation :public Transformation
{
private:
	std::shared_ptr<OGRSpatialReference> m_ptrSrcOGRSpatialReference;
	OGRCoordinateTransformation* m_pOGRTransformation;
public:
	Project2GeoTransformation(int EPSG, int targetEPSG = 4326);
	Project2GeoTransformation(const std::string& wkt, int targetEPSG = 4326);
	virtual ~Project2GeoTransformation();
	virtual void Transform(double* pX, double* pY, double* pZ, int nCount, int offset);
	virtual bool IsValid();
};

/**
 * @brief 地理坐标系到投影坐标系转换
 */
class Geo2ProjectTransformation :public Transformation
{
private:
	std::shared_ptr<OGRSpatialReference> m_ptrSrcOGRSpatialReference;
	OGRCoordinateTransformation* m_pOGRTransformation;
public:
	Geo2ProjectTransformation(int tarEPSG, int srcEPSG = 4326);
	virtual ~Geo2ProjectTransformation();
	virtual void Transform(double* pX, double* pY, double* pZ, int nCount, int offset);
	virtual bool IsValid();
};

/**
 * @brief 地理坐标系到地心坐标系转换
 */
class Geo2GeocentricTransformation :public Transformation
{
public:
	Geo2GeocentricTransformation(int EPSG = 4326);
	virtual ~Geo2GeocentricTransformation();

	virtual void Transform(double* pX, double* pY, double* pZ, int nCount, int offset);
	virtual bool IsValid();
};



/**
 * @brief 模型坐标系统，ENU站心坐标系或者EPGS描述的投影坐标系
 */
class ModelCoordinateSystem
{
public:
	enum class CoordinateSystemType
	{
		eENU,
		eProject,
		eRelative
	};
public:
	ModelCoordinateSystem(const std::string& metafile);
	ModelCoordinateSystem(const Point3d& center);
	ModelCoordinateSystem(double degreeL, double degreeB, double height);
	~ModelCoordinateSystem();

	TransformMatrix GetTransformMatirx();

	Point3<double> GetOrigin();

	virtual void Model2Geo(double* pX, double* pY, double* pZ, size_t num, size_t offset);
	virtual void Geo2Model(double* pX, double* pY, double* pZ, size_t num, size_t offset);

	virtual void Model2Geo(double& x, double& y, double& z);
	virtual void Geo2Model(double& x, double& y, double& z);

	virtual void Model2Geo(BoundingVolumeBox& box);
	virtual void Geo2Model(BoundingVolumeBox& box);

	static TransformMatrix ENUMatrix(const Point3d& pt);

	static TransformMatrix ENUMatrix(double degreeL, double degreeB, double h);

	static TransformMatrix ENURelativeMatrix(const Point3d& from, const Point3d& to);

	static TransformMatrix ENURelativeMatrix(double fromDegreeL, double fromDegreeB, double fromh, double toDegreeL, double toDegreeB, double toh);

private:
	Point3<double>  m_Origin;
	Point3<double>  m_Location;
	CoordinateSystemType m_CoordinateSystemType;
	std::string mMetaDataFile;

	std::shared_ptr<Transformation> m_ptrENU2GeocentricTransformation;
	std::shared_ptr<Transformation> m_ptrGeocentric2ENUTransformation;
	std::shared_ptr<Transformation> m_ptrGeocentric2GeoTransformation;
	std::shared_ptr<Transformation> m_ptrGeo2GeocentricTransformation;
	std::shared_ptr<Transformation> m_ptrProject2GeoTransformation;
	std::shared_ptr<Transformation> m_ptrGeo2ProjectTransformation;

	TransformMatrix m_Transform;
	void Init(const std::string& metadataFile);
};


typedef std::shared_ptr<ModelCoordinateSystem> ModelCoordinateSystemPtr;
typedef std::shared_ptr<Transformation> TransformationPtr;

TransformationPtr CreateTransformation(const std::string& proj);

}
