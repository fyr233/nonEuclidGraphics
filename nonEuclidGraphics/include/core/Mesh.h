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

	struct TextureInfo {
		unsigned id;
		std::string type;
		std::string path;
	};

public:
	Mesh();
	Mesh(std::string path);	// 初始化的时候可以默认参数坐标是自身坐标系下的欧式坐标
	~Mesh();

	void LoadObj(std::string path);
	void Transform(vecf3 center, matf3 S, matf3 R);	//变换结果存在ParaCoord中

	void LoadMesh();				// 向OpenGL中加载网格数据
	//void ParaReset();
	void Draw(GLuint programID, const matf4& m2paraTransform);	// 用着色器绘制

private:
	std::vector<std::string> SplitString(const std::string& s, const std::string& spliter);

public:
	/*  Mesh Data  */
	std::vector<Vertex> vertices;	//点
	std::vector<unsigned int> indices;	//面
	std::vector<TextureInfo> textureInfos;	//贴图

	/*  Render data  */
	unsigned int VAO = 0;
	unsigned int VBO = 0, EBO = 0;

private:
	std::vector<vecf3> positions;
	std::vector<vecf3> normals;
	std::vector<vecf2> texcoords;
};


inline Mesh::Mesh()
{
}

inline Mesh::Mesh(std::string path)
{
	LoadObj(path);
	LoadMesh();
}

inline Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}