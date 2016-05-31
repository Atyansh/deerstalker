#pragma once

#include <assimp/Importer.hpp>

#include "SGeode.h"
#include "Mesh.h"
#include "client\Graphics\Shader.h"
#include <vector>
#include <unordered_map>

using namespace std;

#define POSITION_LOCATION 0
#define NORMAL_LOCATION 1
#define TEX_COORD_LOCATION 2
#define BONE_ID_LOCATION     3
#define BONE_WEIGHT_LOCATION 4

#define MAX_BONES 100

class PlayerMesh : public SGeode
{
private:
	/*  Mesh Data  */
	vector<Vertex> vertices;
	vector<GLuint> indices;
	unordered_map<std::uint32_t, vector<Texture>> texturesMap;
	MaterialNoTex materialNoTex;
	bool hasBones;

	/* Uniform Loc*/
	GLuint modelLoc;
	GLuint viewLoc;
	GLuint projLoc;
	GLuint hasTexLoc;
	GLuint hasBonesLoc;
	vector<GLuint> boneLocs;
	GLuint ambient;
	GLuint diffuse;
	GLuint specular;
	GLuint shininess;


	/*  Render data  */
	GLuint VAO, VBO, EBO;

	Shader *shader;

	void setupMesh();
	void setupUniformLoc();

public:
	PlayerMesh(vector<Vertex> vertices, vector<GLuint> indices, unordered_map<std::uint32_t, vector<Texture>> texturesMap, MaterialNoTex materialNoTex, Shader *shader, bool hasBones);
	virtual ~PlayerMesh(void);

	void setBoneMatrix(GLint index, aiMatrix4x4 matrix);

	virtual void update(UpdateData&);
	virtual void draw(DrawData&);

};