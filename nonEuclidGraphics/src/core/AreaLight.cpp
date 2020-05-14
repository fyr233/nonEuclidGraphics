#include <core/AreaLight.h>
#include <nonEuclideanEngine/world.h>

using namespace cgcore;

AreaLight::AreaLight(nonEuc::World* _world, float intensity, const rgbf& color, Texture2D* texture)
	: Obj(_world), intensity{ intensity }, color{ color }, texture{ texture }
{
	tri.pos[0] = vecf3({ 0, 0, 0 });
	auto G = world->metric(tri.pos[0]);
	auto S = SchmidtOrthogonalize(G);

	tri.pos[1] = S.dot(vecf3({ 1, 0, 0 }));
	tri.pos[2] = S.dot(vecf3({ 0, 1, 0 }));

	tri.obj_id = this->obj_id;

	this->obj_type = Obj::ObjType::_AreaLight;
	genNormal();
}

AreaLight::AreaLight(nonEuc::World* _world, vecf3 position, float intensity, const rgbf& color, Texture2D* texture)
	: Obj(_world) ,intensity{ intensity }, color{ color }, texture{ texture }
{
	tri.pos[0] = position;
	auto G = world->metric(tri.pos[0]);
	auto S = SchmidtOrthogonalize(G);

	tri.pos[1] = S.dot(vecf3({ 1, 0, 0 }));
	tri.pos[2] = S.dot(vecf3({ 0, 1, 0 }));

	tri.obj_id = this->obj_id;

	this->obj_type = Obj::ObjType::_AreaLight;
	genNormal();
}

cgcore::AreaLight::AreaLight(nonEuc::World* _world, vecf3 position, vecf3 edge1, vecf3 edge2, float intensity, const rgbf& color, Texture2D* texture)
	: Obj(_world), intensity{ intensity }, color{ color }, texture{ texture }
{
	tri.pos[0] = position;
	auto G = world->metric(tri.pos[0]);
	auto S = SchmidtOrthogonalize(G);

	tri.pos[1] = S.dot(edge1);
	tri.pos[2] = S.dot(edge2);

	tri.obj_id = this->obj_id;

	this->obj_type = Obj::ObjType::_AreaLight;
	genNormal();
}

AreaLight::~AreaLight()
{
}

float cgcore::AreaLight::getArea()
{
	auto G = world->metric(tri.pos[0]);
	auto S = SchmidtOrthogonalize(G);
	auto Sinv = S.inverse();
	return 0.5f * vecf3::cross(Sinv.dot(tri.pos[0]), Sinv.dot(tri.pos[1])).norm();
}

rgbf AreaLight::Radiance(const vecf2& uv)
{
	if (texture != nullptr)
	{
		return texture->Sample(uv) * intensity;
	}
	else
	{
		return color * intensity;//没有texture
	}
}

void AreaLight::Draw(GLuint programID)
{
	//暂时不予光源以绘制
}

void cgcore::AreaLight::genNormal()
{
	auto G = world->metric(tri.pos[0]);
	auto S = SchmidtOrthogonalize(G);
	auto Sinv = S.inverse();
	tri.normal = S.dot(vecf3::cross(Sinv.dot(tri.pos[1]), Sinv.dot(tri.pos[2])).normalize());
	std::cout << tri.normal;
}
