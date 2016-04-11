#pragma once
// Std. Includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;
// GL Includes
#include <GL/glew.h> // Contains all the necessery OpenGL includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Drawable.h"
#include "Shader.h"

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

	Shader shader;

	void setupMesh();

public:
	Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures, Shader shader);
	virtual ~Mesh(void);

	virtual void update(UpdateData&);
	virtual void draw(DrawData&);

};