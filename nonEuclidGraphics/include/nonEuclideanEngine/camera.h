#pragma once

#include <GL/gl3w.h>            // Initialize with gl3wInit()
#include <GLFW/glfw3.h>

#include <core/mat.h>
#include <core/vec.h>
#include <core/geometry.h>

using namespace cgcore;

class Camera {
public:
	Camera();
	Camera(vecf3 position, matf3 S);
	~Camera();

	void UpdateDirection(vecf3 position, matf3 S);

public:
	vecf3 paraPos;		// 参数坐标下的位置
	vecf3 Front;		// z轴向量(参数坐标)
	vecf3 Up;			// y轴向量
	vecf3 Right;		// x轴向量

	float yaw = -90.0f;	// 水平角
	float pitch = 0.0f; // 俯仰角

private:
	float toRad(float degree){ return degree / 180 * PI<float>; }	
	//matf3 R;
};