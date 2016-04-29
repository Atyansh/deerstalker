#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>


Camera::Camera() {
	position = glm::vec3(0.f, 30.f, 70.f);
	target = glm::vec3(0.0f, 20.0f, 0.f);
	direction = glm::normalize(position - target);

	up = glm::vec3(0.0f, 1.0f, 0.0f);
	right = glm::normalize(glm::cross(up, direction));

	cameraUp = glm::cross(direction, right);
}

Camera::~Camera() {

}

glm::mat4 Camera::getView() {
	glm::mat4 view;
	view = glm::lookAt(position, target, up);
	return view;
}

glm::vec3 Camera::getPosition(){
	return this->position;
}

glm::vec3 Camera::getDirection(){
	return this->direction;
}
