#pragma once

#include <glm/glm.hpp>

class Camera_Original {
private:
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 target;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 cameraUp;
public:

	Camera_Original();
	~Camera_Original();

	glm::mat4 getView();
	glm::vec3 getPosition();
	glm::vec3 getDirection();
};

