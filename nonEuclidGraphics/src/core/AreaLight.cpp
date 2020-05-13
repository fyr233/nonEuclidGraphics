#include <core/AreaLight.h>

using namespace cgcore;

AreaLight::AreaLight(nonEuc::World* _world, float intensity, const rgbf& color, Texture2D* texture)
	: Obj(_world), intensity{ intensity }, color{ color }, texture{ texture }
{
	tri.pos[0] = vecf3({ 0, 0, 0 });
	tri.pos[1] = vecf3({ 1, 0, 0 });
	tri.pos[2] = vecf3({ 0, 1, 0 });

	tri.obj_id = this->obj_id;

	this->obj_type = _AreaLight;
}

AreaLight::AreaLight(nonEuc::World* _world, vecf3 position, float intensity, const rgbf& color, Texture2D* texture)
	: Obj(_world) ,intensity{ intensity }, color{ color }, texture{ texture }
{
	tri.pos[0] = position;
	tri.pos[1] = position + vecf3({ 1, 0, 0 });
	tri.pos[2] = position + vecf3({ 0, 1, 0 });

	tri.obj_id = this->obj_id;

	this->obj_type = _AreaLight;
}

AreaLight::~AreaLight()
{
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