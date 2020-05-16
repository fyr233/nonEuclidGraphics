#pragma once

#include <core/vec.h>
#include <core/mat.h>
#include <core/Mesh.h>
#include <core/Obj.h>
#include <core/transform.h>
#include <core/Material.h>
#include <core/geometry.h>



namespace cgcore
{
	class Object : public Obj
	{
	public:
		~Object();

		Object(nonEuc::World* _world, std::shared_ptr<Mesh> _mesh, vecf3 _center, vecf3 _scale, vecf3 _rotate)
			: Obj(_world), mesh{ _mesh }, center{ _center }, rotate{ _rotate }
		{
			scale = matf3::Diag(_scale);
			obj_type = Obj::ObjType::_Object;
			UpdateR();
		}

		matf3 Getm2paraCoord();
		void UpdateR();
		void Draw(GLuint programID);

	private:
		float toRad(float degree) { return degree / 180 * PI<float>; }

	public:
		std::shared_ptr<Mesh> mesh;
		vecf3 center;
		matf3 R;
		matf3 scale;
		vecf3 color;	//在没有贴图时使用

		vecf3 rotate;

	private:


	};

	inline Object::~Object()
	{
		
	}
}
