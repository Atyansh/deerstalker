#pragma once

#include <glm/glm.hpp>

class Camera {

private:

	glm::mat4 _camMatrix;  //Matrix of the camera
	glm::mat4 _ballMatrix; //Matrix of target
	
	glm::vec3 _ballCamOffset; // Vector of where camera is from ball
	
	glm::vec3 _worldUp;
	glm::vec3 _camUp;

	void pitch(glm::vec3 rotDir); // Move camera up and down
	void yaw(glm::vec3 rotDir);	  // Move camera left and right
	void roll(glm::vec3 rotDir);  

public:	
	Camera();
	~Camera();

	glm::mat4 getView();

	// Gonna pass data from server. Haven't figure it out yet.
	void update(/*TBD*/);
};