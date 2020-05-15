
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
	//GLint loc;
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

std::vector<Triangle> nonEuc::World::GetTriangles()
{
	std::vector<Triangle> ans;
	for (size_t i = 0; i < objectPtrs.size(); i++)
	{
		if (objectPtrs[i]->obj_type == Obj::ObjType::_Object)
		{
			auto pobject = std::static_pointer_cast<Object>(objectPtrs[i]);
			matf3 m2para = pobject->Getm2paraCoord();
			auto m = std::static_pointer_cast<Object>(objectPtrs[i])->mesh;
			for (size_t j = 0; j < m->faces.size(); j++)
			{
				Triangle t;

				t.pos[0] = m2para.dot(m->positions[m->faces[j].v_idx[0]]) + pobject->center;
				t.pos[1] = m2para.dot(m->positions[m->faces[j].v_idx[1]]) + pobject->center - t.pos[0];
				t.pos[2] = m2para.dot(m->positions[m->faces[j].v_idx[2]]) + pobject->center - t.pos[0];

				auto G = metric(t.pos[0]);
				auto S = SchmidtOrthogonalize(G);
				auto Sinv = S.inverse();
				t.normal = S.dot(vecf3::cross(Sinv.dot(t.pos[1]), Sinv.dot(t.pos[2])).normalize());

				t.uv[0] = m->texcoords[m->faces[j].vt_idx[0]];
				t.uv[1] = m->texcoords[m->faces[j].vt_idx[1]];
				t.uv[2] = m->texcoords[m->faces[j].vt_idx[2]];

				t.obj_id = i;

				t.face_id = j;

				ans.push_back(t);
			}
		}
		else // _AreaLight
		{
			Triangle t;
			t = std::static_pointer_cast<AreaLight>(objectPtrs[i])->tri;

			//t.pos done

			//t.normal done

			t.uv[0] = vecf2({ 0, 0 });
			t.uv[1] = vecf2({ 1, 0 });
			t.uv[2] = vecf2({ 0, 1 });

			t.obj_id = i;

			t.face_id = 0;
			ans.push_back(t);
		}
	}
	return ans;
}
