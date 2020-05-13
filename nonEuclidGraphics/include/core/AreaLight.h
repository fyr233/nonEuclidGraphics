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
		AreaLight(nonEuc::World* _world, float intensity = 1.f, const rgbf& color = rgbf(1.f), Texture2D* texture = nullptr);
		AreaLight(nonEuc::World* _world, vecf3 position, float intensity = 1.f, const rgbf& color = rgbf(1.f), Texture2D* texture = nullptr);
			
		~AreaLight();

		vecf3 getLightPos() { return tri.pos[0]; }

		rgbf RadianceFactor() { return  color * intensity; }
		rgbf Radiance(const vecf2& uv) ;

		void Draw(GLuint programID);

	private:


	public:
		//float width;
		//float height;
		Triangle tri;
		float intensity;
		rgbf color;	//rgb
		Texture2D* texture = nullptr;
	};
}