#pragma once

#include <GL/gl3w.h>            // Initialize with gl3wInit()
#include <GLFW/glfw3.h>

#include <core/mat.h>
#include <core/vec.h>
#include <core/transform.h>
#include <core/geometry.h>

namespace nonEuc
{
	class World;
	class Camera;
}

#include <nonEuclideanEngine/world.h>

using namespace cgcore;

namespace nonEuc
{


	class Camera {
	public:
		Camera(vecf3 position, nonEuc::World& _world);
		~Camera();

		void ResetCamera(vecf3 position, float yaw, float pitch);
		void UpdateDirection(float dyaw, float dpitch);
		void UpdatePosition(vecf3 du);
		matf4 GetView();

	public:
		nonEuc::World& world;	//对世界的引用（让camera按需自己去计算S、G，省得总是传递）

		vecf3 paraPos;		// 参数坐标下的位置
		//vecf3 Front;		// z轴向量(参数坐标)
		//vecf3 Up;			// y轴向量
		//vecf3 Right;		// x轴向量

		matf3 Rotation;		// 旋转矩阵	
		matf3 T;		// 位置矩阵	T = (Right, Up, Front)^T = (S * R)^T

	private:
		float toRad(float degree) { return degree / 180 * PI<float>; }
	};
}
