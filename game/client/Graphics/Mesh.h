#pragma once

#include <assimp/Importer.hpp>

#include "SGeode.h"
#include "Shader.h"
#include <vector>

using namespace std;

#define POSITION_LOCATION 0
#define NORMAL_LOCATION 1
#define TEX_COORD_LOCATION 2

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture {
	GLuint id;
	string type;
	aiString path;
};

class Mesh : public SGeode
{
private:
	/*  Mesh Data  */
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;

	/*  Render data  */
	GLuint VAO, VBO, EBO;

	Shader *shader;

	void setupMesh();

public:
	Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures, Shader *shader);
	virtual ~Mesh(void);

	virtual void update(UpdateData&);
	virtual void draw(DrawData&);

	//void printShaderPID() {
	//	fprintf(stderr, "Shader PID in mesh: %u\n", shader->getPid());
	//}

};