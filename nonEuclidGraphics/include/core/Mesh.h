#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#include <core/vec.h>
#include <core/mat.h>


#include <GL/gl3w.h>            // Initialize with gl3wInit()
#include <GLFW/glfw3.h>
#include <core/Texture2D.h>
#include <core/Material.h>

class Mesh
{
	struct Vertex {
		// position
		vecf3 Position;
		// Parameter coordinates
		vecf3 ParaCoord;
		// normal
		vecf3 Normal;
		// texCoord
		vecf2 TexCoord;
		// TBN
		matf3 TBN;
	};

	struct Face
	{
		//position index
		unsigned int v_idx[3];
		//texture index
		unsigned int vt_idx[3];
		//normal index
		unsigned int vn_idx[3];
	};

public:
	Mesh();
	//Mesh(std::string path, Material::MaterialType mtype = Material::MaterialType::DEFAULT);	// 初始化的时候可以默认参数坐标是自身坐标系下的欧式坐标
	// 加载obj和加载纹理图片
	Mesh(std::string path, std::string texImagePath, Material::MaterialType mtype = Material::MaterialType::DEFAULT);
	// 加载obj，并使用提供的纹理
	Mesh(std::string path, std::shared_ptr<Texture2D> pTex, Material::MaterialType mtype = Material::MaterialType::DEFAULT);
	~Mesh();

	void LoadObj(std::string path);

	void LoadMesh();				// 向OpenGL中加载网格数据
	void LoadTexture(std::string path, std::string type = "Albedo");
	//void ParaReset();
	void Draw(GLuint programID, const matf4& m2paraTransform);	// 用着色器绘制

private:
	std::vector<std::string> SplitString(const std::string& s, const std::string& spliter);

public:
	/*  Mesh Data  */

	std::vector<vecf3> positions;
	std::vector<vecf3> normals;
	std::vector<vecf2> texcoords;

	std::vector<unsigned int> indices;//仅用于OpenGL

	//std::vector<Vertex> vertices;		//点
	std::vector<Face> faces;	//面
	std::shared_ptr<Texture2D> AlbedoTexture;		//贴图
	Material material;


	/*  Render data  */
	unsigned int VAO = 0;
	unsigned int VBO = 0, EBO = 0;

private:
	
};


inline Mesh::Mesh()
{
}

/*inline Mesh::Mesh(std::string path, Material::MaterialType mtype)
{
	LoadObj(path);
	LoadMesh();
	material = Material(mtype);
}*/

inline Mesh::Mesh(std::string path, std::string texImagePath, Material::MaterialType mtype)
{
	LoadObj(path);
	LoadMesh();
	LoadTexture(texImagePath, "Albedo");
	material = Material(mtype);
}

inline Mesh::Mesh(std::string path, std::shared_ptr<Texture2D> pTex, Material::MaterialType mtype)
{
	LoadObj(path);
	LoadMesh();
	AlbedoTexture = pTex;
	material = Material(mtype);
}

inline Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}