#pragma once

#include "stdafx.h"
#include "../Core/Model.h"
#include "Pyramid.h"

namespace BBGLOBE {

	//enum class SPATIALINDEXTYPE {
	//	QuadTree,
	//	OcTree,
	//	KDTree
	//};

	class QuadNode {
	public:
		QuadNode() {};
		QuadNode(unsigned int level, unsigned int row, unsigned int col)
			:mLevel(level), mRow(row), mCol(col)
		{

		};
		~QuadNode() {};

		std::vector<ModelPtr>& GetModelPtrArray() {
			return mVecModelPtr;
		}

		using QuadNodePtr = std::shared_ptr<QuadNode>;

	private:
		unsigned int mLevel;			
		unsigned int mRow;
		unsigned int mCol;

		double	mLongitude;
		double  mLatitude;
		double  mHeight;

		QuadNodePtr mParentNode;
		QuadNodePtr mChildNode[4];

		std::vector<ModelPtr>	mVecModelPtr;
	};
	using QuadNodePtr = std::shared_ptr<QuadNode>;


	// 空间索引
	class QuadTreeIndex {
	public:
		QuadTreeIndex(unsigned int startLevel, unsigned int endlevel)
			: mStartLevel(startLevel), mEndLevel(endlevel)
		{
			mRoot = std::make_shared<QuadNode>();
		}
		virtual ~QuadTreeIndex() {

		}

		bool AddModel(ModelPtr modelPtr) {
			double longitude = modelPtr->GetLon();
			double latitude = modelPtr->GetLat();

			unsigned int row = 0;
			unsigned int col = 0;
			Pyramid::instance()->getTile(mEndLevel,Math::Degree2Rad(longitude), Math::Degree2Rad(latitude), row, col);

			Point3i tileInfo(mEndLevel, row, col);
			auto iterTile = mQuadNodeMap.find(tileInfo);
			if (iterTile == mQuadNodeMap.end())
			{
				QuadNodePtr tempNode = std::make_shared<QuadNode>(mEndLevel, row, col);
				tempNode->GetModelPtrArray().push_back(modelPtr);
				mQuadNodeMap[tileInfo] = tempNode;
			}
			else 
			{
				iterTile->second->GetModelPtrArray().push_back(modelPtr);
			}
			return true;
		}

	private:
		QuadNodePtr								mRoot;							// 空间索引根节点
		unsigned int							mStartLevel;					// 开始层级
		unsigned int							mEndLevel;						// 结束层级
		std::map<Point3i, QuadNodePtr>			mQuadNodeMap;					// 采用自底向上的方式构建树时，将瓦片数据存放的容器
	};
	using QuadTreeIndexPtr = std::shared_ptr<QuadTreeIndex>;
}