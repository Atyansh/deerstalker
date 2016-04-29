#pragma once

#include <glm/glm.hpp>

class Camera {
private:
	//glm::vec3 front;
	//glm::vec3 worldUp;
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 target;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 cameraUp;
public:

	Camera();
	//Camera(glm::vec3 pos, glm::vec3 tar, glm::vec3 u);
	~Camera();

	// Get the view matrix for mvp computation
	glm::mat4 getView();
	glm::vec3 getPosition();
	//glm::vec3 getFront();
	glm::vec3 getDirection();
};

