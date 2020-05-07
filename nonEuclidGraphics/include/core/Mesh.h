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
	Mesh(std::string path);	// ��ʼ����ʱ�����Ĭ�ϲ�����������������ϵ�µ�ŷʽ����
	~Mesh();

	void LoadObj(std::string path);
	void Transform(vecf3 center, matf3 S, matf3 R);	//�任�������ParaCoord��

	void LoadMesh();				// ��OpenGL�м�����������
	//void ParaReset();
	void Draw(GLuint programID);	// ����ɫ������

private:
	std::vector<std::string> SplitString(const std::string& s, const std::string& spliter);

public:
	/*  Mesh Data  */
	std::vector<Vertex> vertices;	//��
	std::vector<unsigned int> indices;	//��
	std::vector<TextureInfo> textureInfos;	//��ͼ

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