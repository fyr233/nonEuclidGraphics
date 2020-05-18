#pragma once


//FAST-BVH
#include <FastBVH.h>

#include <core/rgb.h>
#include <nonEuclideanEngine/world.h>

#include <core/primitive.h>
namespace nonEuc
{



	class RayTracer
	{
	public:
		RayTracer() {}

		RayTracer(World* _world);

		~RayTracer()
		{
			delete bvh;
		}

		World* world = nullptr;
		
		FastBVH::BVH<float, Triangle>* bvh = nullptr;

		std::vector<Triangle> triangles;		//我们的所有 primitive 都是 Triangle。world中的所有obj/area/light的三角形都统一放到triangles中

		rgbf background_color;

		float decay_distance;							//衰减系数
		float dt;										//迭代步长
		float distance_limit;

		void SetWorld(World* _world);
		// 设置RayTrace的参数
		// 距离极限，衰减系数，背景颜色，迭代步长
		void SetParameter(float _distance_limit, float _decay_distance, rgbf _background_color, float _dt);
		// 生成world中的所有三角形，生成BVH
		void BuildBVH();
		
		// 渲染几何关系
		cv::Mat RenderTracing(float fov, float aspect, int width);

		int blocksize = 8;

	private:
		




		//求解一条光线的着色器
		//ray:射线及其方向, distance:光线已走距离（距离发出为止）, times 光线反射的次数
		rgbf shader(FastBVH::Ray<float> ray, float distance, int times);
		rgbf tracer(FastBVH::Ray<float> ray, FastBVH::Traverser<float, Triangle, Intersector>& traverser, float distance);

		void blocktracer(FastBVH::Traverser<float, Triangle, Intersector>& traverser,
			const vecf3& o, const vecf3& cameraz, const vecf3& camerax, const vecf3& cameray,
			float delta,
			int ibegin, int iend, int jbegin, int jend,
			int midwid, int midhgt,
			cv::Mat& img);
	};
}