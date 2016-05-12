#include "Mesh.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/ext.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Mesh::Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures, MaterialNoTex materialNoTex, Shader *shader, bool hasBones=false) : SGeode()
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;
	this->materialNoTex = materialNoTex;
	this->shader = shader;
	this->hasBones = hasBones;

	this->setupMesh();
	this->setupUniformLoc();
}

Mesh::~Mesh()
{

}

void Mesh::draw(DrawData& data)
{

	if (!shader->isInitilized()){
		cerr << "Shader not initialized" << endl;
		exit(-1);
	}

	shader->bind();

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(data.matrix));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(data.view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(data.projection));
	glUniform1i(hasTexLoc, this->textures.size() > 0 );
	glUniform1i(hasBonesLoc, this->hasBones);

	if (this->textures.size() > 0) {
		// Bind appropriate textures
		GLuint diffuseNr = 1;
		GLuint specularNr = 1;
		for (GLuint i = 0; i < this->textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i); // Active proper texture unit before binding
			// Retrieve texture number (the N in diffuse_textureN)
			stringstream ss;
			string number;
			string name = this->textures[i].type;
			if (name == "texture_diffuse")
				ss << diffuseNr++; // Transfer GLuint to stream
			else if (name == "texture_specular")
				ss << specularNr++; // Transfer GLuint to stream
			number = ss.str();
			// Now set the sampler to the correct texture unit
			glUniform1i(glGetUniformLocation(shader->getPid(), (name + number).c_str()), i);
			// And finally bind the texture
			glBindTexture(GL_TEXTURE_2D, this->textures[i].id);

			// Also set each mesh's shininess property to a default value (if you want you could extend this to another mesh property and possibly change this value)
			glUniform1f(glGetUniformLocation(shader->getPid(), "material.shininess"), 16.0f);
		}
	} else {
		glUniform3fv(ambient, 1, glm::value_ptr(this->materialNoTex.ambient));
		glUniform3fv(diffuse, 1, glm::value_ptr(this->materialNoTex.diffuse));
		glUniform3fv(specular, 1, glm::value_ptr(this->materialNoTex.specular)); // Specular doesn't have full effect on this object's material
		glUniform1f(shininess, materialNoTex.shininess);
	}
	
	//// Also set each mesh's shininess property to a default value (if you want you could extend this to another mesh property and possibly change this value)
	//glUniform1f(glGetUniformLocation(shader->getPid(), "material.shininess"), 16.0f);
	

	// Draw mesh
	glBindVertexArray(this->VAO);
	glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	// Always good practice to set everything back to defaults once configured.
	for (GLuint i = 0; i < this->textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	shader->unbind();
}

void Mesh::update(UpdateData& data)
{
	//
}

// Sets bone transformation matrices
void Mesh::setBoneMatrix(GLint index, aiMatrix4x4 matrix)
{
	glm::mat4 mat = glm::transpose(glm::make_mat4(&matrix.a1));
	glUniformMatrix4fv(boneLocs[index], 1, GL_FALSE, glm::value_ptr(mat));
}

void Mesh::setupMesh(){
	// Create buffers/arrays
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);

	glBindVertexArray(this->VAO);
	// Load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

	// Set the vertex attribute pointers
	// Vertex Positions
	glEnableVertexAttribArray(POSITION_LOCATION);
	glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	// Vertex Normals
	glEnableVertexAttribArray(NORMAL_LOCATION);
	glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
	// Vertex Texture Coords
	if (this->textures.size() > 0) {
		glEnableVertexAttribArray(TEX_COORD_LOCATION);
		glVertexAttribPointer(TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));
	}

	if (this->hasBones) {
		glEnableVertexAttribArray(BONE_ID_LOCATION);
		glVertexAttribIPointer(BONE_ID_LOCATION, 4, GL_INT, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, BoneIds));

		glEnableVertexAttribArray(BONE_WEIGHT_LOCATION);
		glVertexAttribPointer(BONE_WEIGHT_LOCATION, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const GLvoid*)offsetof(Vertex, Weights));
	}

	glBindVertexArray(0);
}

void Mesh::setupUniformLoc() {
	if (!shader->isInitilized()) {
		cerr << "Shader not initialized" << endl;
		exit(-1);
	}

	shader->bind();

	modelLoc = glGetUniformLocation(shader->getPid(), "model");
	viewLoc = glGetUniformLocation(shader->getPid(), "view");
	projLoc = glGetUniformLocation(shader->getPid(), "projection");
	hasTexLoc = glGetUniformLocation(shader->getPid(), "hasTexture");
	hasBonesLoc = glGetUniformLocation(shader->getPid(), "hasBones");
	if (this->hasBones) {
		for (size_t i = 0; i < MAX_BONES; i++) {
			string tmp = "bones[" + to_string(i) + "]";
			boneLocs.push_back(glGetUniformLocation(shader->getPid(), tmp.c_str()));
		}
	}

	ambient = glGetUniformLocation(shader->getPid(), "materialNoTex.ambient");
	diffuse = glGetUniformLocation(shader->getPid(), "materialNoTex.diffuse");
	specular = glGetUniformLocation(shader->getPid(), "materialNoTex.specular");
	shininess = glGetUniformLocation(shader->getPid(), "materialNoTex.shininess");

	shader->unbind();
}
