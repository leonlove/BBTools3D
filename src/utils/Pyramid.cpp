#include "Pyramid.h"
#include "glm/ext/scalar_constants.hpp"
#include "glm/gtc/constants.hpp"
#include <math.h>
#include "MathUtil.h"

namespace BBGLOBE
{

PyramidKey::PyramidKey()
{
    Level = 0;
    Row = 0;
    Col = 0;
}
PyramidKey::PyramidKey(unsigned int l, unsigned int r, unsigned int c)
{
    Level = l;
    Row = r;
    Col = c;
}
PyramidKey::PyramidKey(const PyramidKey& rhs)
{
    Level = rhs.Level;
    Row = rhs.Row;
    Col = rhs.Col;
}
bool PyramidKey::operator<(const PyramidKey& rhs) const
{
    return (Level != rhs.Level) ? (Level < rhs.Level) :
        (Row != rhs.Row) ? (Row < rhs.Row) :
        (Col < rhs.Col);

}
bool PyramidKey::operator !=(const PyramidKey& rhs) const
{
    return Level != rhs.Level || Row != rhs.Row || Col != rhs.Col;
}
bool PyramidKey::operator ==(const PyramidKey& rhs) const
{
    return Level == rhs.Level && Row == rhs.Row && Col == rhs.Col;
}

std::vector<PyramidKey> PyramidKey::Children() const
{
    std::vector<PyramidKey> children;
    int r = Row * 2;
    int c = Col * 2;
	children.reserve(4);
    children.emplace_back(Level + 1, r, c);
    children.emplace_back(Level + 1, r, c + 1);
    children.emplace_back(Level + 1, r + 1, c);
    children.emplace_back(Level + 1, r + 1, c + 1);
    return children;
}
    // 获取瓦片的父瓦片Key
PyramidKey PyramidKey::Parent() const
{
    return PyramidKey(Level - 1, Row / 2, Col / 2);
}

const Pyramid *Pyramid::instance(void)
{
    static Pyramid s_Pyramid;
    return &s_Pyramid;
}

Pyramid::Pyramid(void)
{
	m_ptOrigin.x = -glm::pi<double>();
	m_ptOrigin.y = -glm::half_pi<double>();
	m_vecTopTileSize.x = glm::pi<double>() + glm::pi<double>();
	m_vecTopTileSize.y = glm::pi<double>() + glm::pi<double>();
    init();
}


Pyramid::Pyramid(double OX, double OY, double TopTileSizeX, double TopTileSizeY)
{
	m_ptOrigin.x = OX;
	m_ptOrigin.y = OY;
	m_vecTopTileSize.x = TopTileSizeX;
	m_vecTopTileSize.y = TopTileSizeY;
    init();
}


//Pyramid::Pyramid(const Pyramid &Obj)
//{
//    *this = Obj;
//}
//
//const Pyramid &Pyramid::operator = (const Pyramid &Obj)
//{
//    if(this == &Obj)    return *this;
//    *this = Obj;
//    return *this;
//}

const glm::dvec2 &Pyramid::getOrigin(void) const
{
    return m_ptOrigin;
}


const glm::dvec2 &Pyramid::getTopTileSize(void) const
{
    return m_vecTopTileSize;
}


const glm::dvec2 &Pyramid::getLevelTileSize(unsigned int nLevel) const
{
    if (nLevel >= MAXLEVEL)
    {
        static const glm::dvec2 vec(0.0, 0.0);
        return vec;
    }
    return m_LevelTileSizes[nLevel];
}


unsigned int Pyramid::getMaxCol(unsigned int nLevel) const
{
    return 1u << nLevel;
}


unsigned int Pyramid::getMaxRow(unsigned int nLevel) const
{
    return 1u << nLevel;
}

unsigned int Pyramid::getBestLevel(double lonMin, double latMin, double lonMax, double latMax) const
{
	// 差值小于网格
	double lonSize = abs(lonMax - lonMin);
	double latSize = abs(latMax - latMin);
	for (int i = MAXLEVEL - 1; i >= 0; i--)
	{
		if (m_LevelTileSizes[i].x > lonSize && m_LevelTileSizes[i].y > latSize)
			return i;
	}
	return 0;
}


bool Pyramid::getTilePos(unsigned int nLevel, unsigned int nRow, unsigned int nCol, double &xMin, double &yMin, double &xMax, double &yMax) const
{
    if (nLevel >= MAXLEVEL)
    {
        return false;
    }

    xMin = m_ptOrigin.x + (nCol * m_LevelTileSizes[nLevel].x);
    yMin = m_ptOrigin.y + (nRow * m_LevelTileSizes[nLevel].y);
    xMax = xMin + m_LevelTileSizes[nLevel].x;
    yMax = yMin + m_LevelTileSizes[nLevel].y;
    if (xMin > xMax)
    {
		Math::Swap(xMin, xMax);
    }
    if (yMin > yMax)
    {
		Math::Swap(yMin, yMax);
    }
    return true;
}


bool Pyramid::getTile(unsigned int nLevel, double x, double y, unsigned int &nRow, unsigned int &nCol) const
{
    if (nLevel >= MAXLEVEL)
    {
        return false;
    }

    nCol = (unsigned int)floor((x - m_ptOrigin.x) / m_LevelTileSizes[nLevel].x);
    nRow = (unsigned int)floor((y - m_ptOrigin.y) / m_LevelTileSizes[nLevel].y);
    return true;
}


bool Pyramid::getTile(unsigned int nLevel, double xMin, double yMin, double xMax, double yMax, unsigned int&nRowMin, unsigned int&nColMin, unsigned int&nRowMax, unsigned int&nColMax) const
{
    if (nLevel >= MAXLEVEL)
    {
        return false;
    }

    getTile(nLevel, xMin, yMin, nRowMin, nColMin);
    getTile(nLevel, xMax, yMax, nRowMax, nColMax);
    if (nRowMin > nRowMax)
    {
		Math::Swap(nRowMin, nRowMax);
    }
    if (nColMin > nColMax)
    {
		Math::Swap(nColMin, nColMax);
    }
    return true;
}


bool Pyramid::getTile(double xMin, double yMin, double xMax, double yMax, unsigned int &nLevel, unsigned int &nRow, unsigned int &nCol) const
{
    if (nLevel >= MAXLEVEL)
    {
        return false;
    }
	bool result = false;
    unsigned int nRowMin, nRowMax, nColMin, nColMax;
	for (unsigned int i = MAXLEVEL-1; i >=0 ; i--)
    {
        getTile(i, xMin, yMin, xMax, yMax, nRowMin, nColMin, nRowMax, nColMax);
        if ((nRowMin == nRowMax) && (nColMin == nColMax))
        {
            nRow = nRowMin;
            nCol = nColMin;
            nLevel = i;
			result = true;
			break;
        }
    }
    return result;
}

bool Pyramid::getParent(unsigned int nLevel, unsigned int nRow, unsigned int nCol, unsigned int &nLevelParent, unsigned int &nRowParent, unsigned int &nColParent) const
{
    if(nLevel == 0u)
    {
        nLevelParent = ~0u;
        nRowParent = ~0u;
        nColParent = ~0u;
        return false;
    }

    nLevelParent = --nLevel;
    nRowParent = nRow >> 1u;
    nColParent = nCol >> 1u;
    return true;
}


//通过层号行号列号，得到父亲节点的层号行号列号
bool Pyramid::getParentByLevel(unsigned int nLevel, unsigned int nRow, unsigned int nCol, unsigned int nLevelParent, unsigned int &nRowParent, unsigned int &nColParent) const
{
    if(nLevelParent > nLevel || nLevel == 0u)
    {
        nRowParent = ~0u;
        nColParent = ~0u;
        return false;
    }

	if (nLevelParent == nLevel)
	{
		nRowParent = nRow;
		nColParent = nCol;
		return false;
	}

    const unsigned int nLevelDetal = nLevel - nLevelParent;
    nRowParent = nRow >> nLevelDetal;
    nColParent = nCol >> nLevelDetal;
    return true;
}


void Pyramid::init()
{
    m_LevelTileSizes[0u] = m_vecTopTileSize;
    for(unsigned int i = 1u; i < MAXLEVEL; i++)
    {
        m_LevelTileSizes[i] = m_LevelTileSizes[i - 1u] * 0.5;
    }
}

}

