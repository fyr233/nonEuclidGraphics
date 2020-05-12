#pragma once

#include <core/vec.h>
#include <core/mat.h>
#include <core/Mesh.h>
#include <core/primitive.h>
#include <core/transform.h>
#include <core/Texture2D.h>
#include <core/Obj.h>


namespace cgcore
{
	class AreaLight : public Obj
	{
	public:
		//AreaLight();
		AreaLight(float intensity = 1.f, const rgbf& color = rgbf{ 1.f }, Texture2D* texture = nullptr);
			
		~AreaLight();

		rgbf RadianceFactor() { return  color * intensity; }
		rgbf Radiance(const vecf2& uv) ;

		virtual void Draw(GLuint programID);

	private:


	public:
		//float width;
		//float height;
		Triangle tri;
		float intensity;
		rgbf color;	//rgb
		Texture2D* texture = nullptr;
	};

	AreaLight::AreaLight(float intensity, const rgbf& color, Texture2D* texture)
		: intensity{ intensity }, color{ color }, texture{ texture }
	{
		tri.pos[0] = vecf3({ 0, 0, 0 });
		tri.pos[1] = vecf3({ 1, 0, 0 });
		tri.pos[2] = vecf3({ 0, 1, 0 });
		tri.idx[0] = 0;
		tri.idx[1] = 1;
		tri.idx[2] = 2;
		tri.obj_id = this->obj_id;

		this->obj_type = _AreaLight;
	}

	AreaLight::~AreaLight()
	{
	}

	inline rgbf AreaLight::Radiance(const vecf2& uv)
	{
		if (texture != nullptr)
		{
			return texture->Sample(uv) * intensity;
		}
		else
		{
			return color * intensity;//Ã»ÓÐtexture
		}
	}
	inline void AreaLight::Draw(GLuint programID)
	{
	}
}