#pragma once

#include <core/vec.h>
#include <core/mat.h>
#include <core/Mesh.h>
#include <core/transform.h>

namespace nonEuc
{
	class World;
}

#include <nonEuclideanEngine/world.h>


namespace cgcore
{
	class Obj
	{
	public:
		~Obj();

		Obj(std::shared_ptr<Mesh> _mesh, vecf3 _center, vecf3 _scale, matf3 _rotation, nonEuc::World& _world)
			:mesh{ _mesh }, center{ _center }, R{ _rotation }, world{ _world }
		{
			scale = matf3::Diag(_scale);
		}

		void Transform(matf3 S);
		void Draw(GLuint programID);
	private:


	public:
		std::shared_ptr<Mesh> mesh;
		vecf3 center;
		matf3 R;
		matf3 scale;
		vecf3 color;	//在没有贴图时使用
		nonEuc::World& world;

	private:


	};

	inline Obj::~Obj()
	{

	}
}
