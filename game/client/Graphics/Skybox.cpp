#include "Skybox.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "../Globals.h"

const char* vert = "Graphics/Assets/Cubemap/Skybox.vert";
const char* frag = "Graphics/Assets/Cubemap/Skybox.frag";

Skybox::Skybox()
{
}


Skybox::~Skybox()
{
}

void Skybox::setupVAO() {
	shader = new Shader(vert, frag);

	float skyboxSize = 7000.f;

	GLfloat skyboxVertices[] = {
		// Positions          
		-skyboxSize, skyboxSize, -skyboxSize,
		-skyboxSize, -skyboxSize, -skyboxSize,
		skyboxSize, -skyboxSize, -skyboxSize,
		skyboxSize, -skyboxSize, -skyboxSize,
		skyboxSize, skyboxSize, -skyboxSize,
		-skyboxSize, skyboxSize, -skyboxSize,

		-skyboxSize, -skyboxSize, skyboxSize,
		-skyboxSize, -skyboxSize, -skyboxSize,
		-skyboxSize, skyboxSize, -skyboxSize,
		-skyboxSize, skyboxSize, -skyboxSize,
		-skyboxSize, skyboxSize, skyboxSize,
		-skyboxSize, -skyboxSize, skyboxSize,

		skyboxSize, -skyboxSize, -skyboxSize,
		skyboxSize, -skyboxSize, skyboxSize,
		skyboxSize, skyboxSize, skyboxSize,
		skyboxSize, skyboxSize, skyboxSize,
		skyboxSize, skyboxSize, -skyboxSize,
		skyboxSize, -skyboxSize, -skyboxSize,

		-skyboxSize, -skyboxSize, skyboxSize,
		-skyboxSize, skyboxSize, skyboxSize,
		skyboxSize, skyboxSize, skyboxSize,
		skyboxSize, skyboxSize, skyboxSize,
		skyboxSize, -skyboxSize, skyboxSize,
		-skyboxSize, -skyboxSize, skyboxSize,

		-skyboxSize, skyboxSize, -skyboxSize,
		skyboxSize, skyboxSize, -skyboxSize,
		skyboxSize, skyboxSize, skyboxSize,
		skyboxSize, skyboxSize, skyboxSize,
		-skyboxSize, skyboxSize, skyboxSize,
		-skyboxSize, skyboxSize, -skyboxSize,

		-skyboxSize, -skyboxSize, -skyboxSize,
		-skyboxSize, -skyboxSize, skyboxSize,
		skyboxSize, -skyboxSize, -skyboxSize,
		skyboxSize, -skyboxSize, -skyboxSize,
		-skyboxSize, -skyboxSize, skyboxSize,
		skyboxSize, -skyboxSize, skyboxSize
	};

	skyboxVAO = GLuint();
	skyboxVBO = GLuint();

	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glBindVertexArray(0);
	
	cubemap = Cubemap::loadCubemap("Graphics/Assets/Cubemap");
}

void Skybox::draw() {

	glDepthFunc(GL_LEQUAL);
	shader->bind();
	glUniformMatrix4fv(glGetUniformLocation(shader->getPid(), "view"), 1, GL_FALSE, glm::value_ptr(Globals::drawData.view));
	glUniformMatrix4fv(glGetUniformLocation(shader->getPid(), "projection"), 1, GL_FALSE, glm::value_ptr(Globals::drawData.projection));

	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(shader->getPid(), "skybox"), 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	glDepthFunc(GL_LESS); // Set depth function back to default

	shader->unbind();
}