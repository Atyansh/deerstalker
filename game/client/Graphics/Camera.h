#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>

class Camera {

private:

	glm::mat4 _objectMat;

	glm::mat4 _camMatrix;  //Matrix of the camera
	
	glm::vec3 _ballCamOffset; // Vector of where camera is from ball

	void update(glm::mat4 rotMat);

public:	
	Camera();
	~Camera();

	glm::mat4 getView();

	// Gonna pass data from server. Haven't figure it out yet.
	void pitch(int up); // Move camera up and down
	void yaw(int up);	  // Move camera left and right
	void roll(int up);

	glm::vec3 getPosition() { return glm::vec3(_camMatrix[3]); }
	void updateCamObjectMat(glm::mat4 mat, glm::mat4 world) { _objectMat = world * mat; }
};