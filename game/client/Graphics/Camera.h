#pragma once

#include <glm/glm.hpp>

class Camera {
private:
	glm::vec3 position;		// Position of the camera
	glm::vec3 front;		// Distance from the postion to the center
	glm::vec3 direction;	// Please note that the direction from the target to the camera
	glm::vec3 worldUp;

public:

	Camera();
	Camera(glm::vec3 pos, glm::vec3 tar, glm::vec3 u);
	~Camera();

	// Get the view matrix for mvp computation
	glm::mat4 getView();

};

