#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>

class Camera {

private:
	int playerId_;
	glm::vec3 _objectVec;
	glm::vec3 _camVec;  //Matrix of the camera
	
	void update(glm::mat4 rotMat, float lookUpMin, float lookDownMin);

public:	
	Camera();
	~Camera();

	glm::mat4 getView();

	// Gonna pass data from server. Haven't figure it out yet.
	void pitch(int up); // Move camera up and down
	void yaw(int up);	  // Move camera left and right
	void roll(int up);

	int getPlayerId() {
		return playerId_;
	}

	void setPlayerId(int playerId) {
		playerId_ = playerId;
	}

	glm::vec3 getPosition() { return glm::vec3(_camVec); }
	void updateCamObjectMat(glm::vec3 pos) { _objectVec = pos + glm::vec3(0, 5, 0); }
	glm::vec3 getCamDirection();
};