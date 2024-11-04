#pragma once
#include "glm/vec2.hpp"
#include <memory>
#include <vector>
#include <set>

namespace BBGLOBE
{

/**
 * 金字塔键值，层行列
 */
class PyramidKey
{
public:
	unsigned int Level;
	unsigned int Row;
	unsigned int Col;

	PyramidKey();
	PyramidKey(unsigned int l, unsigned int r, unsigned int c);
	PyramidKey(const PyramidKey& rhs);
	bool operator<(const PyramidKey& rhs) const;
	bool operator !=(const PyramidKey& rhs) const;
	bool operator ==(const PyramidKey& rhs) const;

	/**
	 * \brief 获取子瓦片
	 */
	std::vector<PyramidKey> Children() const;
	PyramidKey Parent() const;

	std::set<PyramidKey> ChildrenWithData;
};


class Pyramid
{
	const static unsigned int MAXLEVEL = 32u;

public:
	Pyramid(void);
	//构造金字塔对象，传入参数
	//OX,OY: 原点坐标
	//XAxis, YAxis: X轴方向和Y轴方向，true表示正方向，false表示负方向
	//TopTileSizeX,TopTileSizeY: 顶层一个瓦片的大小，实际大小
	//TilePixelX,TilePixelY: 瓦片像素大小
	//LevelScale: 每层见的比例关系
	Pyramid(double OX, double OY, double TopTileSizeX, double TopTileSizeY);
	Pyramid(const Pyramid& Obj);

	static const Pyramid* instance(void);

public:
	const Pyramid& operator = (const Pyramid& Obj);

public:
	const glm::dvec2& getOrigin(void) const;
	const glm::dvec2& getTopTileSize(void) const;
	const glm::dvec2& getLevelTileSize(unsigned int nLevel) const;

	//通过层号和行列号得到瓦片的范围
	bool getTilePos(unsigned int nLevel, unsigned int nRow, unsigned int nCol, double& xMin, double& yMin, double& xMax, double& yMax) const;

	//通过层号和点坐标得到行列号
	bool getTile(unsigned int nLevel, double x, double y, unsigned int& nRow, unsigned int& nCol) const;

	//通过层号和数据范围得到行列号范围
	bool getTile(unsigned int nLevel, double xMin, double yMin, double xMax, double yMax, unsigned int& nRowMin, unsigned int& nColMin, unsigned int& nRowMax, unsigned int& nColMax) const;

	//通过数据范围，判断哪一层，哪一个瓦片，正好包含数据
	bool getTile(double xMin, double yMin, double xMax, double yMax, unsigned int& nLevel, unsigned int& nRow, unsigned int& nCol) const;

	//通过层号行号列号，得到父亲节点的层号行号列号
	bool getParent(unsigned int nLevel, unsigned int nRow, unsigned int nCol, unsigned int& nLevelParent, unsigned int& nRowParent, unsigned int& nColParent) const;

	//通过层号行号列号和指定的父亲层号，父亲层的节点的行号列号，要求父亲层号小于当前层号
	bool getParentByLevel(unsigned int nLevel, unsigned int nRow, unsigned int nCol, unsigned int nLevelParent, unsigned int& nRowParent, unsigned int& nColParent) const;

	// 获取某一层的最大行列号
	unsigned int getMaxCol(unsigned int nLevel) const;
	unsigned int getMaxRow(unsigned int nLevel) const;

	// 根据经纬度范围(弧度)获取最佳层级
	unsigned int getBestLevel(double lonMin, double latMin, double lonMax, double latMax) const;
protected:
	void init();

protected:
	glm::dvec2  m_ptOrigin;
	glm::dvec2  m_vecTopTileSize;
	glm::dvec2  m_LevelTileSizes[MAXLEVEL];
};
using PyramidPtr = std::shared_ptr<Pyramid>;
}
