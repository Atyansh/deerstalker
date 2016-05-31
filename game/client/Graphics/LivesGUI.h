#pragma once

#include "Shader.h"
#include <glm/glm.hpp>

class LivesGUI {
private:
	glm::mat4 _projection;

	GLuint VAO, VBO;
	Shader *shader;

public:
	LivesGUI();
	LivesGUI(Shader *s, glm::mat4 proj) : shader(s), _projection(proj) { setupBuffers(); }
	~LivesGUI();

	void setupBuffers();

	void draw(glm::mat4 translation);
	void update();
};

