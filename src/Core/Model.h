#pragma once

#include <string>
#include <vector>
#include "mesh.h"
#include "../utils/Types.h"
#include "../utils/Transformation.h"

namespace BBGLOBE {

	/**
	* @brief 自定义三维数据基类
	*/
	class Model {
	public:
		Model();
		virtual ~Model();


		//// 返回数据类型
		//std::string getType() const override {
		//	return "Model";
		//}

		MemberFun(const std::string&, Name);

		MemberFun(double, Lon);
		MemberFun(double, Lat);
		MemberFun(double, Height);

		void AddMesh(MeshPtr mesh);

		// 计算模型包围盒
		void DirtyBound();

		// 坐标转换——将网格的顶点转换为以地理坐标为中心点的局部坐标
		void CoordinateTransform();

		ModelCoordinateSystemPtr GetModelCoordinateSystem();

	private:
		std::string						mName;							// 模型名称
		double							mLon;							// 模型地理坐标-经度
		double							mLat;							// 模型地理坐标-纬度
		double							mHeight;						// 模型地理坐标-高度

		std::vector<MeshPtr>			mMeshs;							// 模型的所有网格数据
		BoundingVolumeBox				mBoundingBox;					// 模型包围盒

		ModelCoordinateSystemPtr		mModelCoordinateSystemPtr;		// 模型坐标转换系统
	};

	using ModelPtr = std::shared_ptr<Model>;
	using ModelPtrArray = std::vector<ModelPtr>;
}
