#include "Camera.h"

#include <glm/gtx/string_cast.hpp>

#include <iostream>

Camera::Camera() {
	_ballCamOffset = glm::vec3(0.0f, 0.0f, 20.0f);
	_camMatrix = glm::translate(glm::mat4(), _ballCamOffset);

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

	_camMatrix = rotMat * _camMatrix;
}

glm::mat4 Camera::getView() {
	return glm::lookAt(glm::vec3(_camMatrix[3]) + glm::vec3(_objectMat[3]), glm::vec3(_objectMat[3]), glm::vec3(0.0f, 1.0f, 0.0f));
}

