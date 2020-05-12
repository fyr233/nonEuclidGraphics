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
		//参数坐标的位置（paraCoord）
		vecf3 pos[3];
		//在world中所属的物体编号
		size_t obj_id;
		//所属物体的顶点编号（如果是光源的话，0, 1, 2 即可）
		size_t idx[3];
	};
}
