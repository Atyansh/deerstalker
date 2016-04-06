#ifndef _CUBE_H_
#define _CUBE_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Cube
{
public:
	Cube(float);
	~Cube();

	glm::mat4 toWorld;

	float size;

	void draw();
	void update();

	void spin(float);
};

#endif

