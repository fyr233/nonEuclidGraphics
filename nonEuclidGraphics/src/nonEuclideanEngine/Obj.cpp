
#include<core/Obj.h>
#include<nonEuclideanEngine/world.h>

using namespace cgcore;

void Obj::Transform(matf3 S)
{
	mesh->Transform(center, S, R);
}

void Obj::Draw(GLuint programID)
{
	// ¼ÆËã±ä»»
	matf3 S = cgcore::SchmidtOrthogonalize(world.metric(center));
	matf4 transform = cgcore::MakeTransform(S, R, scale, center);
	mesh->Draw(programID, transform);
}