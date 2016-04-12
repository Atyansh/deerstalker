#pragma once

#include <assimp/Importer.hpp>

#include "Drawable.h"
#include "Shader.h"

using namespace std;

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

class Mesh : public Drawable
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