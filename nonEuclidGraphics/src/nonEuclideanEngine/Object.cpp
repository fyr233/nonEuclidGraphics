
#include<core/Object.h>
#include<nonEuclideanEngine/world.h>

using namespace cgcore;

void Object::Transform(matf3 S)
{
	mesh->Transform(center, S, R);
}

void Object::Draw(GLuint programID)
{
	// 计算变换
	matf3 G = world->metric(center);
	matf3 S = cgcore::SchmidtOrthogonalize(G);

	matf4 transform = cgcore::MakeTransform(S, R, scale, center);

	glUseProgram(programID);

	// 传入该物体位置的变换矩阵计算反射光
	GLint Location = glGetUniformLocation(programID, "SR");
	glUniformMatrix3fv(Location, 1, GL_TRUE, (S * R).data);
	Location = glGetUniformLocation(programID, "G");
	glUniformMatrix3fv(Location, 1, GL_TRUE, G.data);
	//std::cout << S * R << std::endl;
	//system("pause");

	mesh->Draw(programID, transform);
}