#pragma once


#include <core/mat.h>
#include <core/func.h>
#include <core/Mesh.h>
#include <core/Obj.h>
#include <nonEuclideanEngine/camera.h>

namespace nonEuc
{
	class World
	{
	public:
		//描述整个空间（三维流形）的性质

		//规范化坐标（限制每个点参数坐标唯一）
		Func3 regularize_ref;
		Func3to3 regularize;
		
		//三维流形曲纹坐标->四维空间的映射(不一定用到)
		Func3to4 coord;
		//度规矩阵
		Func3to33 metric;
		//Jacobi矩阵(不一定用到)
		Func3to43 jacobi;
		//度量的Christoffel记号
		Func3to333 gamma;
	
		std::vector<std::shared_ptr<Obj>> objectPtrs;
		Camera camera;

	public:
		World();
		~World();
		void AddObj(std::shared_ptr<Mesh> newMesh, vecf3 center, matf3 rotation);
	};

	inline World::World()
	{
		camera = Camera(cgcore::vecf3{0.0f, 0.0f, -10.0f}, cgcore::matf3::Identity());
	}

	inline World::~World()
	{

	}
}

