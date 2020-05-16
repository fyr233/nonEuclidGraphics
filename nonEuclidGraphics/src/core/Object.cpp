
#include<core/Object.h>
#include<nonEuclideanEngine/world.h>
#include <core/gl.h>
using namespace cgcore;

matf3 Object::Getm2paraCoord()
{
	matf3 S = cgcore::SchmidtOrthogonalize(world->metric(center));
	return (S * R) * scale;
}

void Object::Draw(GLuint programID)
{
	// ¼ÆËã±ä»»
	matf3 S = cgcore::SchmidtOrthogonalize(world->metric(center));
	matf4 transform = cgcore::MakeTransform(S, R, scale, center);
	gl::SetMat3f(programID, "SR", S * R);
	mesh->Draw(programID, transform);
}

void Object::UpdateR()
{
	R = matf3{
		1, 0, 0,
		0, cos(toRad(rotate[0])), -sin(toRad(rotate[0])),
		0, sin(toRad(rotate[0])), cos(toRad(rotate[0])),
	} * matf3{
		cos(toRad(rotate[1])), 0, -sin(toRad(rotate[1])),
		0, 1, 0,
		sin(toRad(rotate[1])), 0, cos(toRad(rotate[1])),
	} * matf3{
		cos(toRad(rotate[2])), -sin(toRad(rotate[2])), 0,
		sin(toRad(rotate[2])), cos(toRad(rotate[2])), 0,
		0, 0, 1,
	};
}