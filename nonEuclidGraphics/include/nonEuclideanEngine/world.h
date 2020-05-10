#pragma once

#include <type_traits>
#include <core/mat.h>
#include <core/func.h>
#include <core/Mesh.h>
namespace cgcore
{
	class Obj;
}

#include <core/Obj.h>

#include <nonEuclideanEngine/camera.h>
#include <nonEuclideanEngine/WorldExample.h>
#include <core/transform.h>

namespace nonEuc
{
	class Camera;

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
		Camera* camera = nullptr;

	public:

		World();

		//使用一个WorldExample来设置世界
		template<typename TWorldExample>
		void SetWorldExample();

		~World();
		void AddObj(std::shared_ptr<Mesh> newMesh, vecf3 center, vecf3 scale, matf3 rotation);
	};

	template<typename TWorldExample>
	void World::SetWorldExample()
	{
		//从WorldExample中应用世界
		static_assert(std::is_base_of<WorldExample::WorldExampleBase, TWorldExample>::value);
		coord = TWorldExample::coord;
		jacobi = TWorldExample::jacobi;
		gamma = TWorldExample::gamma;
		regularize_ref = TWorldExample::regularize_ref;
		regularize = TWorldExample::regularize;
		metric = TWorldExample::metric;

		//初始化Camera (初始化方法可以修改)
		delete camera;
		camera = new Camera({ 0.f, 0.f, 3.f }, *this);
	}

	inline World::~World()
	{
		delete camera;
	}
}

