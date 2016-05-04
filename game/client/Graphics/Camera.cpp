#include "Camera.h"

#include <glm/gtx/string_cast.hpp>

#include <iostream>

Camera::Camera() {
	_ballMatrix = glm::mat4();
	std::cout << "_ballMatrix:\n" << glm::to_string(_ballMatrix) << std::endl;
	_camMatrix = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, 20.0f));
	std::cout << "_camMatrix:\n" << glm::to_string(_camMatrix) << std::endl;
}

Camera::~Camera() {}

void Camera::pitch(int up) {
	if (up == 0) {
		_ballMatrix = glm::rotate(_ballMatrix, 0.020f, glm::vec3(1.0f, 0.0f, 0.0f));
	}
	else {
		_ballMatrix = glm::rotate(_ballMatrix, -0.020f, glm::vec3(1.0f, 0.0f, 0.0f));
	}
	_camMatrix = _ballMatrix * (glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, 20.f)));
	std::cout << "_ballMatrix:\n" << glm::to_string(_ballMatrix) << std::endl;
	std::cout << "_camMatrix:\n" << glm::to_string(_camMatrix) << std::endl;
}

void Camera::yaw(int up) {
	if (up == 0) {
		_ballMatrix = glm::rotate(_ballMatrix, 0.020f, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else {
		_ballMatrix = glm::rotate(_ballMatrix, -0.020f, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	//_camMatrix = _ballMatrix * _camMatrix;
	_camMatrix = _ballMatrix * (glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, 20.f)));
	std::cout << "_ballMatrix:\n" << glm::to_string(_ballMatrix) << std::endl;
	std::cout << "_camMatrix:\n" << glm::to_string(_camMatrix) << std::endl;
}

void Camera::roll(int up) {
	if (up == 0) {
		_ballMatrix = glm::rotate(_ballMatrix, 0.020f, glm::vec3(0.0f, 0.0f, 1.0f));
	}
	else {
		_ballMatrix = glm::rotate(_ballMatrix, -0.020f, glm::vec3(0.0f, 0.0f, 1.0f));

	}
	//_camMatrix = _ballMatrix * _camMatrix;
	_camMatrix = _ballMatrix * (glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, 20.f)));
	std::cout << "_ballMatrix:\n" << glm::to_string(_ballMatrix) << std::endl;
	std::cout << "_camMatrix:\n" << glm::to_string(_camMatrix) << std::endl;
}

void Camera::update() {

}

glm::mat4 Camera::getView() {
	return glm::lookAt(glm::vec3(_camMatrix[3]), glm::vec3(_ballMatrix[3]), glm::vec3(0.0f, 1.0f, 0.0f));
}

