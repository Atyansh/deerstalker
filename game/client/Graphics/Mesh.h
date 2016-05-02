#pragma once

#include <assimp/Importer.hpp>

#include "SGeode.h"
#include "Shader.h"
#include <vector>

using namespace std;

#define POSITION_LOCATION 0
#define NORMAL_LOCATION 1
#define TEX_COORD_LOCATION 2
#define BONE_ID_LOCATION     3
#define BONE_WEIGHT_LOCATION 4

#define MAX_BONES 100

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::ivec4 BoneIds;
	glm::vec4 Weights;
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
	bool hasBones;

	/* Uniform Loc*/
	GLuint modelLoc;
	GLuint viewLoc;
	GLuint projLoc;
	GLuint hasTexLoc;
	GLuint hasBonesLoc;
	vector<GLuint> boneLocs;

	/*  Render data  */
	GLuint VAO, VBO, EBO;

	Shader *shader;

	void setupMesh();
	void setupUniformLoc();

public:
	Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures, Shader *shader, bool hasBones);
	virtual ~Mesh(void);

	void setBoneMatrix(GLint index, aiMatrix4x4 matrix);

	virtual void update(UpdateData&);
	virtual void draw(DrawData&);

	//void printShaderPID() {
	//	fprintf(stderr, "Shader PID in mesh: %u\n", shader->getPid());
	//}

};