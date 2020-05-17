#pragma once

#include <type_traits>
#include <core/mat.h>
#include <core/func.h>
#include <core/Mesh.h>

#include <core/Obj.h>
#include <core/Object.h>

#include <core/AreaLight.h>

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
		//Func3i regularize_ref;
		//Func3ito3 regularize;
		Func3ijk regularize_ref;
		Func3ijkto3 regularize;
		
		//度规矩阵
		Func3to33 metric;
		//度量的Christoffel记号
		Func3to333 gamma;
	
		std::vector<std::shared_ptr<Obj>> objectPtrs;
		Camera camera;


	public:

		World();

		// 使用一个WorldExample来设置世界
		template<typename TWorldExample>
		void SetWorldExample();

		~World();
		// 添加网格物体，并指定center, scale, rotation
		void AddObj(std::shared_ptr<Mesh> newMesh, vecf3 center, vecf3 scale, vecf3 rotation);
		//
		void AddAreaLight(vecf3 position, vecf3 edge1, vecf3 edge2, float intensity, rgbf color);
		// 设置OpenGL的光源
		void SetUniformLight(GLuint programID);
		// 获取世界中所有三角形, 包括Object和AreaLight
		std::vector<Triangle> GetTriangles();

	private:

		
	};

	template<typename TWorldExample>
	void World::SetWorldExample()
	{
		//从WorldExample中应用世界
		static_assert(std::is_base_of<WorldExample::WorldExampleBase, TWorldExample>::value);
		gamma = TWorldExample::gamma;
		regularize_ref = TWorldExample::regularize_ref;
		regularize = TWorldExample::regularize;
		metric = TWorldExample::metric;

		//初始化Camera (初始化方法可以修改)
		camera = Camera({ 0.f , PI<float>/2.f, PI<float>/2.f}, this);

	}

	inline World::~World()
	{

	}
}

