#include "Camera.h"

#include <glm/gtx/string_cast.hpp>

#include <iostream>

Camera::Camera() {
	_camVec = glm::vec3(0.0f, 0.0f, 20.0f);
}

Camera::~Camera() {}

void Camera::pitch(int up) {
	glm::mat4 rotMat;

	if (up == 0) {
		rotMat = glm::rotate(rotMat, 0.020f, glm::vec3(1.0f, 0.0f, 0.0f));
	}
	else {
		rotMat = glm::rotate(rotMat, -0.020f, glm::vec3(1.0f, 0.0f, 0.0f));
	}
	
	update(rotMat);
}

void Camera::yaw(int up) {
	glm::mat4 rotMat;

	if (up == 0) {
		rotMat = glm::rotate(rotMat, 0.020f, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else {
		rotMat = glm::rotate(rotMat, -0.020f, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	
	update(rotMat);
}

void Camera::roll(int up) {
	glm::mat4 rotMat;

	if (up == 0) {
		rotMat = glm::rotate(rotMat, 0.020f, glm::vec3(0.0f, 0.0f, 1.0f));
	}
	else {
		rotMat = glm::rotate(rotMat, -0.020f, glm::vec3(0.0f, 0.0f, 1.0f));
	}

	update(rotMat);
}

void Camera::update(glm::mat4 rotMat) {
	glm::vec4 pos = rotMat * glm::vec4(_camVec, 0);
	_camVec = glm::vec3(pos);
}

glm::mat4 Camera::getView() {
	return glm::lookAt((_camVec + _objectVec), _objectVec, glm::vec3(0.0f, 1.0f, 0.0f));
}

