#pragma once

#include <core/vec.h>
#include <core/mat.h>
#include <core/Mesh.h>


class Obj
{
public:
	Obj();
	~Obj();

	void Transform(matf3 S);

private:


public:
	Mesh mesh;
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

inline void Obj::Transform(matf3 S)
{
	mesh.Transform(center, S, R);
}
