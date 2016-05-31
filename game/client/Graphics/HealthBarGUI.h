#pragma once

#include "Shader.h"
#include <glm/glm.hpp>

class HealthBarGUI {
private:
	float _health; // User health. Between 0.0 and 1.0

	glm::mat4 _projection;

	GLuint baseVAO, baseVBO;
	Shader *shader;

public:

	HealthBarGUI(Shader *s, glm::mat4 proj) : shader(s), _projection(proj) { _health = 1.0f; setupBuffers(); }
	~HealthBarGUI();
	void init();
	void setupBuffers();

	void draw(glm::mat4 translation);
	void update(float health);
};

