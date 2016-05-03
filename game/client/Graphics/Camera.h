#pragma once

#include <glm/glm.hpp>

class Camera {

private:
	glm::vec3 camPosition;
	glm::vec3 ballPosition;
	glm::vec3 worldUp;


public:	
	Camera();
	~Camera();

};