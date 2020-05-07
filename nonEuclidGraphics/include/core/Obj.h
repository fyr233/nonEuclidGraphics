#pragma once

#include <core/vec.h>
#include <core/mat.h>
#include <core/Mesh.h>


class Obj
{
public:
	Obj();
	~Obj();

	Obj(std::shared_ptr<Mesh> _mesh, vecf3 _center, matf3 _rotation)
		:mesh{ _mesh }, center{ _center }, R{ _rotation }{}

	void Transform(matf3 S);

private:


public:
	std::shared_ptr<Mesh> mesh;
	vecf3 center;
	matf3 R;
	vecf3 color;	//在没有贴图时使用

private:


};

inline Obj::Obj()
{

}

inline Obj::~Obj()
{

}