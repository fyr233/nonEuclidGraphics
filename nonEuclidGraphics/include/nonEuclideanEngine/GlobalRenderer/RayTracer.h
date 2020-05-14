#pragma once


//FAST-BVH
#include <FastBVH/BVH.h>

#include <core/rgb.h>
#include <nonEuclideanEngine/world.h>

#include <core/primitive.h>
namespace nonEuc
{



	class RayTracer
	{
	public:
		World& world;
		
		FastBVH::BVH<float, Triangle> bvh;

		std::vector<Triangle> triangles;		//我们的所有 primitive 都是 Triangle。world中的所有obj/area/light的三角形都统一放到triangles中

		float decay;							//衰减系数

	private:
		
		// 生成world中的所有三角形，生成BVH
		void BuildBVH();

		//求解一条光线的着色器
		//ray:射线及其方向, distance:光线已走距离（距离发出为止）, times 光线反射的次数
		rgbf shader(FastBVH::Ray<float> ray, float distance, int times);
	};
}