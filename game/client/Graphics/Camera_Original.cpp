#include "Camera_Original.h"

#include <glm/gtc/matrix_transform.hpp>


Camera_Original::Camera_Original() {
	position = glm::vec3(0.f, 30.f, 70.f);
	target = glm::vec3(0.0f, 20.0f, 0.f);
	direction = glm::normalize(position - target);

	up = glm::vec3(0.0f, 1.0f, 0.0f);
	right = glm::normalize(glm::cross(up, direction));

	cameraUp = glm::cross(direction, right);
}

Camera_Original::~Camera_Original() {

}

glm::mat4 Camera_Original::getView() {
	glm::mat4 view;
	view = glm::lookAt(position, target, up);
	return view;
}

glm::vec3 Camera_Original::getPosition(){
	return this->position;
}

glm::vec3 Camera_Original::getDirection(){
	return this->direction;
}
