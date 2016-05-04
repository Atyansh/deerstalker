#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>

class Camera {

private:

	glm::mat4 _camMatrix;  //Matrix of the camera
	glm::mat4 _ballMatrix; //Matrix of target
	
	glm::vec3 _ballCamOffset; // Vector of where camera is from ball
	
	glm::vec3 _worldUp;
	glm::vec3 _camUp;

	

public:	
	Camera();
	~Camera();

	glm::mat4 getView();

	// Gonna pass data from server. Haven't figure it out yet.
	void update(/*TBD*/);
	void pitch(int up); // Move camera up and down
	void yaw(int up);	  // Move camera left and right
	void roll(int up);

	glm::vec3 getPosition() { return glm::vec3(_camMatrix[3]); }
};