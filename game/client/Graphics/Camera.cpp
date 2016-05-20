#include "Camera.h"

#include <glm/gtx/string_cast.hpp>

#include <iostream>
#include <cmath>


Camera::Camera() {
	_camVec = glm::vec3(0.0f, 0.0f, 20.0f);
}
Camera::~Camera() {}

void Camera::pitch(int up) {
	glm::mat4 rotMat;

	glm::vec3 a = glm::normalize(_camVec);
	glm::vec3 b(0.0f, 1.0f, 0.0f);

	glm::vec3 r = glm::cross(a, b);

	if (up == 0) {
		rotMat = glm::rotate(rotMat, -0.020f, r);
	}
	else {
		rotMat = glm::rotate(rotMat, 0.020f, r);
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

	glm::vec3 dir = glm::normalize(glm::vec3(pos));
	std::cout << "Vec: " << glm::to_string(dir) << std::endl;
	std::cout << "Object Vec: " << glm::to_string(_objectVec) << std::endl;

	if (fabs(dir.x) > 0.2f || fabs(dir.z) > 0.2f) {
		_camVec = glm::vec3(pos);

	}
}

glm::mat4 Camera::getView() {
	return glm::lookAt((_camVec + _objectVec), _objectVec, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::vec3 Camera::getCamDirection() {
	glm::vec3 dir = _camVec;
	dir.y = 0;
	return glm::normalize(dir);
}