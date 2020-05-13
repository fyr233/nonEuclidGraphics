#include <core/Material.h>

Material::Material(MaterialType type)
{
	// see http://www.barradeau.com/nicoptere/dump/materials.html
	switch (type)
	{
	case Material::COPPER:
		ambient = vecf3{ 0.19125f, 0.0735f, 0.0225f };
		diffuse = vecf3{ 0.7038f, 0.27048f, 0.0828f };
		specular = vecf3{ 0.256777f, 0.137622f, 0.086014f };
		shininess = 12.8f;
		break;
	case Material::BRASS:
		ambient = vecf3{ 0.329412f, 0.223529f, 0.027451f };
		diffuse = vecf3{ 0.780392f, 0.568627f, 0.113725f };
		specular = vecf3{ 0.992157f, 0.941176f, 0.807843f };
		shininess = 27.8974f;
		break;
	case Material::BRONZE:
		ambient = vecf3{ 0.2125f, 0.1275f, 0.054f };
		diffuse = vecf3{ 0.714f, 0.4284f, 0.18144f };
		specular = vecf3{ 0.393548f, 0.271906f, 0.166721f };
		shininess = 25.6f;
		break;
	case Material::DEFAULT:
	default:
		ambient = vecf3{0.1f, 0.1f, 0.1f};
		diffuse = vecf3{1.0f, 1.0f, 1.0f};
		specular = vecf3{0.3f, 0.3f, 0.3f};
		shininess = 30.f;
		break;
	}
}