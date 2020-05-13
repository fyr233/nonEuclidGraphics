#pragma once

#include <core/vec.h>

using namespace cgcore;

class Material {
public:
	enum MaterialType
	{
		DEFAULT,
		COPPER,
		BRASS,
		BRONZE,
	};

	Material() {};
	~Material() {};
	Material(vecf3 _ambient, vecf3 _diffuse, vecf3 _specular, float _shininess)
		: ambient{ _ambient }, diffuse{ _diffuse }, specular{ _specular }, shininess{ _shininess }{}
	Material(MaterialType type);
public:
	vecf3 ambient;
	vecf3 diffuse;
	vecf3 specular;
	float shininess;
};
