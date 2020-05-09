#pragma once

#include <core/vec.h>
#include <core/mat.h>
#include <core/Mesh.h>
#include <core/transform.h>

namespace cgcore
{
	class Texture2D
	{
	public:
		Texture2D();
		Texture2D(const std::string& path);
		~Texture2D();

		void Load(const std::string& path);
		vecf3 Sample(vecf2 uv);

	private:


	public:
		enum class WrapMode { Clamp, Repeat, Mirror };
		enum class SampleMode { Nearest, Linear };

		//Image image;

	private:

	};

	Texture2D::Texture2D()
	{
	}

	inline Texture2D::Texture2D(const std::string& path)
	{
		Load(path);
	}

	Texture2D::~Texture2D()
	{
	}

	inline void Texture2D::Load(const std::string& path)
	{
		//TODO
	}

	inline vecf3 Texture2D::Sample(vecf2 uv)
	{
		//TODO
		return vecf3();
	}
}