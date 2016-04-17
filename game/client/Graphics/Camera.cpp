#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>


Camera::Camera() {

	// Default position, target and up
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	front = glm::vec3(0.0f, 0.0f, -20.0f);
	direction = glm::normalize(front);
	worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
}

Camera::Camera(glm::vec3 pos, glm::vec3 tar, glm::vec3 world_up) {

	position = pos;
	front = tar;
	direction = glm::normalize(front);
	worldUp = world_up;

}

Camera::~Camera() {

}

glm::mat4 Camera::getView() {

	return glm::lookAt(position, position + front, worldUp);

}

glm::vec3 Camera::getPosition(){
	return this->position;
}

glm::vec3 Camera::getFront(){
	return this->front;
}

glm::vec3 Camera::getDirection(){
	return this->direction;
}
