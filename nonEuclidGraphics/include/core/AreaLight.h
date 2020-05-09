#pragma once

#include <core/vec.h>
#include <core/mat.h>
#include <core/Mesh.h>
#include <core/transform.h>
#include <core/Texture2D.h>

namespace cgcore
{
	class AreaLight
	{
	public:
		AreaLight();
		AreaLight(float intensity = 1.f, const vecf3& color = vecf3{ 1.f }) //, Texture2D* texture = nullptr)
			: intensity{ intensity }, color{ color }{}
		~AreaLight();

		vecf3 RadianceFactor() { return  color * intensity; }
		vecf3 Radiance(const vecf2& uv) ;

	private:


	public:
		float width;
		float height;
		float intensity;
		vecf3 color;	//rgb
		Texture2D* texture = nullptr;
	};

	AreaLight::AreaLight()
	{
	}

	AreaLight::~AreaLight()
	{
	}

	inline vecf3 AreaLight::Radiance(const vecf2& uv)
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
}