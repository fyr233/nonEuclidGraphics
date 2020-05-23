
#include <nonEuclideanEngine/world.h>
#include <core/gl.h>

using namespace nonEuc;

World::World()
{
	
}

void World::AddObj(std::shared_ptr<Mesh> newMesh, vecf3 center, vecf3 scale, vecf3 rotation)
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

void World::DeleteObj(Object* pobj)
{
	for (auto iter = objectPtrs.begin(); iter != objectPtrs.end(); iter++)
	{
		if ((*iter)->obj_id == pobj->obj_id)
		{
			objectPtrs.erase(iter);
			break;
		}
	}
	if (!pobj)
		delete pobj;
}

World::World(int num)
{
	std::shared_ptr<Texture2D> tex = std::make_shared<Texture2D>();
	tex->Load("../data/test.png");
	tex->SetTextureImage();
	switch (num)
	{
	case 0:// Euclidean
		SetWorldExample<nonEuc::WorldExample::Euclidean>();
		for (int i = -1; i <= 1; i += 2)
			for (int j = -1; j <= 1; j += 2)
				for (int k = -1; k <= 1; k += 2)
					AddObj(std::make_shared<Mesh>("../data/ball.obj", tex, Material::MaterialType::DEFAULT), { (float)i / 2.f, (float)j / 2.f, (float)k / 2.f}, { 0.05f, 0.05f, 0.05f }, vecf3{ 0.f, 0.f, 0.f });
		AddAreaLight({ 0.f, 1.f, 0.f }, { 0.3f, 0.f, 0.f }, { 0.f, 0.f, 0.3f }, 50.f, { 1.f,1.f,1.f });
		camera = Camera({ 0.0f , 0.f, 1.0f }, this);
		break;
	case 1:// Gaussian
		SetWorldExample<nonEuc::WorldExample::Gaussian>();
		for (int i = -1; i <= 1; i += 2)
			for (int j = -1; j <= 1; j += 2)
				for (int k = -1; k <= 1; k += 2)
					AddObj(std::make_shared<Mesh>("../data/ball.obj", tex, Material::MaterialType::DEFAULT), { (float)i / 2.f, (float)j / 2.f, (float)k / 2.f }, { 0.05f, 0.05f, 0.05f }, vecf3{ 0.f, 0.f, 0.f });
		AddAreaLight({ 0.f, 1.f, 0.f }, { 0.3f, 0.f, 0.f }, { 0.f, 0.f, 0.3f }, 50.f, { 1.f,1.f,1.f });
		camera = Camera({ 0.0f , 0.f, 1.0f }, this);
		break;
	case 2:// Hypersphere
		SetWorldExample<nonEuc::WorldExample::HyperSphere>();
		AddObj(std::make_shared<Mesh>("../data/ball.obj", tex, Material::MaterialType::DEFAULT), { PI<float>, PI<float> / 2.f, PI<float> / 2.f }, { 0.05f, 0.05f, 0.05f }, vecf3{ 0.f, 0.f, 0.f });
		AddAreaLight({ PI<float> / 2.f, PI<float> / 2.f+0.1f, PI<float> / 2.f }, { 0.3f, 0.f, 0.f }, { 0.f, 0.f, 0.3f }, 50.f, { 1.f,1.f,1.f });
		camera = Camera({ 0.f, PI<float> / 2.f, PI<float> / 2.f }, this);
		break;
	case 3:// OneRecursive
		SetWorldExample<nonEuc::WorldExample::OneRecursive>();
		AddObj(std::make_shared<Mesh>("../data/ball.obj", tex, Material::MaterialType::DEFAULT), { 0.f, 0.f, 0.f }, { 0.05f, 0.05f, 0.05f }, vecf3{ 0.f, 0.f, 0.f });
		AddAreaLight({ 0.f, 1.f, 0.f }, { 0.3f, 0.f, 0.f }, { 0.f, 0.f, 0.3f }, 50.f, { 1.f,1.f,1.f });
		camera = Camera({ 0.0f , 0.f, 1.0f }, this);
		break;
	case 4:// Hyperbolic3
	{
		std::shared_ptr<Texture2D> tex = std::make_shared<Texture2D>();
		tex->Load("../data/blackedge.png");
		tex->SetTextureImage();
		SetWorldExample<nonEuc::WorldExample::Hyperbolic3>();
		auto pmesh = std::make_shared<Mesh>("../data/cube1.obj", tex, Material::MaterialType::DEFAULT);
		for (int i = 1; i < 25; i++)
		{
			AddObj(pmesh, { PI<float> / 2 + 0.1f, PI<float> / 2,log(i * 0.15f + 1) }, { 0.05f, 0.05f, 0.05f }, vecf3{ 0.f, 0.f, 0.f });
			AddObj(pmesh, { PI<float> / 2 + 0.1f, PI<float> / 2, -log(i * 0.15f + 1) }, { 0.05f, 0.05f, 0.05f }, vecf3{ 0.f, 0.f, 0.f });

			AddObj(pmesh, { PI<float> / 2 - 0.1f, PI<float> / 2,log(i * 0.15f + 1) }, { 0.05f, 0.05f, 0.05f }, vecf3{ 0.f, 0.f, 0.f });
			AddObj(pmesh, { PI<float> / 2 - 0.1f, PI<float> / 2,-log(i * 0.15f + 1) }, { 0.05f, 0.05f, 0.05f }, vecf3{ 0.f, 0.f, 0.f });

			if (i % 10 == 0)
			{
				AddAreaLight({ 0.0f, PI<float> / 2+0.05f, log(i * 0.15f + 1) }, { 0.f, 0.3f, 0.f }, { 0.f, 0.f, 0.3f }, 50.f, { 1.f,1.f,1.f });
				AddAreaLight({ 0.0f, PI<float> / 2+0.05f, -log(i * 0.15f + 1) }, { 0.f, 0.3f, 0.f }, { 0.f, 0.f, 0.3f }, 50.f, { 1.f,1.f,1.f });
			}
		}
		AddObj(pmesh, { PI<float> / 2 + 0.1f, PI<float> / 2,0 }, { 0.05f, 0.05f, 0.05f }, vecf3{ 0.f, 0.f, 0.f });
		AddObj(pmesh, { PI<float> / 2 - 0.1f, PI<float> / 2,0 }, { 0.05f, 0.05f, 0.05f }, vecf3{ 0.f, 0.f, 0.f });
		camera = Camera({ PI<float> / 2, PI<float> / 2, 0.0f }, this);
		break;
	}
	case 5:// Schwarzschild
		SetWorldExample<nonEuc::WorldExample::Schwarzschild>();
		break;
	default:
		break;
	}
}