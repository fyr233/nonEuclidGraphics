#pragma once

#include <core/vec.h>
#include <core/mat.h>
#include <core/rgb.h>
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
		Obj();
		~Obj();

		virtual void Draw(GLuint programID);

	private:


	public:
		enum ObjType
		{
			_Object,
			_AreaLight
		};

		ObjType obj_type;

		//在world中所属的物体编号
		size_t obj_id;

	private:


	};

	inline Obj::Obj()
	{
	}

	inline Obj::~Obj()
	{
	}
	
	inline void Obj::Draw(GLuint programID)
	{

	}
}