#pragma once

#include <core/vec.h>

/*
	为了方便，我们暂时只用一种Primitive， Triangle；（其它的比如Sphere, Square...）
*/

namespace cgcore
{
	//三角形
	struct Triangle
	{
		// pos[0]是0号点的参数坐标，pos[1]、pos[2] 是相对参数坐标 
		vecf3 pos[3];
		// 法线
		vecf3 normal;
		// 纹理坐标
		vecf2 uv[3];
		// 在world中所属的物体编号
		size_t obj_id;
		// 在对应obj中面id
		size_t face_id;
	};
}
