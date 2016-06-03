#include "Camera.h"

#include <glm/gtx/string_cast.hpp>

#include <iostream>
#include <cmath>


Camera::Camera() {
	_camVec = glm::vec3(0.0f, 0.0f, 30.0f);
}

Camera::~Camera() {
}

void Camera::pitch(int up) {
	glm::mat4 rotMat;

	glm::vec3 a = glm::normalize(_camVec);
	glm::vec3 b(0.0f, 1.0f, 0.0f);

	glm::vec3 r = glm::cross(a, b);

	if (up == 0) {
		rotMat = glm::rotate(rotMat, 0.040f, r);
	}
	else {
		rotMat = glm::rotate(rotMat, -0.040f, r);
	}

	update(rotMat, 0.99f, 0.3f);
}

void Camera::yaw(int up) {
	glm::mat4 rotMat;

	if (up == 0) {
		rotMat = glm::rotate(rotMat, -0.040f, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	else {
		rotMat = glm::rotate(rotMat, 0.040f, glm::vec3(0.0f, 1.0f, 0.0f));
	}

	update(rotMat, 0, 0);
}


void Camera::roll(int up) {
	glm::mat4 rotMat;

	if (up == 0) {
		rotMat = glm::rotate(rotMat, 0.040f, glm::vec3(0.0f, 0.0f, 1.0f));
	}
	else {
		rotMat = glm::rotate(rotMat, -0.040f, glm::vec3(0.0f, 0.0f, 1.0f));
	}

	update(rotMat, 0, 0);
}

void Camera::update(glm::mat4 rotMat, float lookUpMin, float lookDownMin) {
	glm::vec4 pos = rotMat * glm::vec4(_camVec, 0);

	glm::vec3 dir = glm::normalize(glm::vec3(pos));
	std::cerr << "Vec: " << glm::to_string(dir) << std::endl;
	std::cerr << "Object Vec: " << glm::to_string(_objectVec) << std::endl;

	if ((fabs(dir.x) > lookDownMin || fabs(dir.z) > lookDownMin) && dir.y >= 0) {
		_camVec = glm::vec3(pos);
	}
	else if ((fabs(dir.x) > lookUpMin || fabs(dir.z) > lookUpMin) && dir.y < 0) {
		_camVec = glm::vec3(pos);
	}
}

void Camera::resetCam() {
	_camVec = glm::vec3(0.0f, 0.0f, 30.0f);
	_objectVec = glm::vec3(0.f);
}

glm::mat4 Camera::getView() {
	return glm::lookAt((_camVec + _objectVec), _objectVec, glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::vec3 Camera::getCamDirection() {
	glm::vec3 dir = _camVec;
	dir.y = 0;
	return glm::normalize(dir);
}