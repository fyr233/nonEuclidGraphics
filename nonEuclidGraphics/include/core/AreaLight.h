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
		AreaLight(nonEuc::World* _world, vecf3 position, vecf3 edge1, vecf3 edge2, float intensity = 1.f, const rgbf& color = rgbf(1.f), Texture2D* texture = nullptr);

		~AreaLight();

		vecf3 getLightPos() { return tri.pos[0] + tri.pos[1] * 0.333f + tri.pos[2] * 0.333f; }
		vecf3 getNormal() { return tri.normal; }
		float getArea();
		rgbf RadianceFactor() { return  color * intensity; }
		rgbf Radiance(const vecf2& uv) ;

		void Draw(GLuint programID);

	private:
		// 计算三角形的法向量
		void genNormal();

	public:
		//float width;
		//float height;
		Triangle tri;
		float intensity;
		rgbf color;	//rgb
		Texture2D* texture = nullptr;
	};
}