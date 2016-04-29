#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>


Camera::Camera() {
	// Default position, target and up
	//front = glm::vec3(0.0f, -10.0f, -10.0f);
	//direction = glm::normalize(glm::vec3(0.0f, 0.0f, 0.0f));
	//worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

	position = glm::vec3(0.f, 10.f, 50.f);
	target = glm::vec3(0.0f, 0.0f, 0.f);
	direction = glm::normalize(position - target);

	up = glm::vec3(0.0f, 1.0f, 0.0f);
	right = glm::normalize(glm::cross(up, direction));

	cameraUp = glm::cross(direction, right);
}
/*
Camera::Camera(glm::vec3 pos, glm::vec3 tar, glm::vec3 world_up) {

	position = pos;
	front = tar;
	direction = glm::normalize(front);
	worldUp = world_up;

}
*/
Camera::~Camera() {

}

glm::mat4 Camera::getView() {
	//return glm::lookAt(position, position + front, worldUp);
	//return glm::lookAt(glm::vec3(0.0f, -6.0f, -20.0f), glm::vec3(0.0f, 0.0f, 0.0f), worldUp);
	glm::mat4 view;
	view = glm::lookAt(position, target, up);
	return view;
}

glm::vec3 Camera::getPosition(){
	return this->position;
}
/*
glm::vec3 Camera::getFront(){
	return this->front;
}
*/
glm::vec3 Camera::getDirection(){
	return this->direction;
}
