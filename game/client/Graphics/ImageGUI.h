#pragma once

#include <glm\glm.hpp>
#include <SOIL\SOIL.h>

#include "Shader.h"

class ImageGUI {

private:
	Shader* _shader;
	glm::mat4 _projection;
	GLuint VAO, VBO, EBO;
	GLuint textureID;
	const char* _path;
	int width, height;


public:
	ImageGUI(const char* imagePath, Shader *shader, int w, int h);
	~ImageGUI();

	void setupBuffers();
	void draw();

};

