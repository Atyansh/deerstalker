#pragma once

#include "Cubemap.h"
#include "Shader.h"

class Skybox {

private:
	GLuint skyboxVAO;
	GLuint skyboxVBO;

	GLuint cubemap;

	Shader *shader;
	
public:
	Skybox();
	~Skybox();

	void setupVAO(string directory);
	void draw();
};

