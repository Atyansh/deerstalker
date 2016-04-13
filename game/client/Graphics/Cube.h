#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Cube {
public:
	int id;
	Cube(float);
	~Cube();

	glm::mat4 toWorld;

	float size;

	void draw();
	void update();

	void spin(float);

	bool holdUp;
	bool holdDown;
	bool holdLeft;
	bool holdRight;
};

