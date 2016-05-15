#pragma once

#include "btBulletDynamicsCommon.h"
#include "Client.h"
#include "DynamicCharacterController.h"

#include "BulletDynamics/Character/btCharacterControllerInterface.h"

class Player {
public:
	Player(btCollisionObject* body);
	~Player();

	DynamicCharacterController* getController();

private:
	DynamicCharacterController* controller_;
};
