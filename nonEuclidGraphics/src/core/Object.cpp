
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