
#include <nonEuclideanEngine/world.h>
#include <core/gl.h>

using namespace nonEuc;

World::World()
{
	
}

void World::AddObj(std::shared_ptr<Mesh> newMesh, vecf3 center, vecf3 scale, matf3 rotation)
{
	objectPtrs.push_back(std::make_shared<Object>(this, newMesh, center, scale, rotation));
}

void nonEuc::World::AddAreaLight(vecf3 position, vecf3 edge1, vecf3 edge2, float intensity, rgbf color)
{
	objectPtrs.push_back(std::make_shared<AreaLight>(this, position, edge1, edge2, intensity, color, nullptr));
}

void nonEuc::World::SetUniformLight(GLuint programID)
{
	int num_Light = 0;
	GLint loc;
	std::string var_name;
	for (auto pObj : objectPtrs)
	{
		if (pObj->obj_type == Obj::_AreaLight)
		{
			AreaLight* pLight = dynamic_cast<AreaLight*>(&(*pObj));
			vecf3 lightpos = pLight->getLightPos();
			vecf3 normal = pLight->getNormal();
			rgbf radiance = pLight->RadianceFactor();
			float area = pLight->getArea();
			var_name = std::string("areaLights[") + char(num_Light + '0') + std::string("].position");
			gl::SetVec3f(programID, var_name.c_str(), lightpos);
			var_name = std::string("areaLights[") + char(num_Light + '0') + std::string("].radiance");
			gl::TSetVec3f(programID, var_name.c_str(), radiance);
			var_name = std::string("areaLights[") + char(num_Light + '0') + std::string("].normal");
			gl::SetVec3f(programID, var_name.c_str(), normal);
			var_name = std::string("areaLights[") + char(num_Light + '0') + std::string("].area");
			gl::SetFloat(programID, var_name.c_str(), area);
			num_Light++;
		}		
	}
	gl::SetInt(programID, "numAreaLights", num_Light);
}
