#pragma once

#include <core/vec.h>
#include <core/mat.h>
#include <core/Mesh.h>
#include <core/Obj.h>
#include <core/transform.h>
#include <core/Material.h>



namespace cgcore
{
	class Object : public Obj
	{
	public:
		~Object();

		Object(nonEuc::World* _world, std::shared_ptr<Mesh> _mesh, vecf3 _center, vecf3 _scale, matf3 _rotation)
			: Obj(_world), mesh{ _mesh }, center{ _center }, R{ _rotation }
		{
			scale = matf3::Diag(_scale);
			obj_type = Obj::ObjType::_Object;
		}

		matf3 Getm2paraCoord();
		void Draw(GLuint programID);

	private:


	public:
		std::shared_ptr<Mesh> mesh;
		vecf3 center;
		matf3 R;
		matf3 scale;
		vecf3 color;	//在没有贴图时使用

	private:


	};

	inline Object::~Object()
	{
		
	}
}
