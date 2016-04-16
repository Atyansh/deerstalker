#pragma once

#include "btBulletDynamicsCommon.h"
#include "Client.h"

class Player : btRigidBody {
public:
	Player(btRigidBodyConstructionInfo& info);
	~Player();

	static Player* createNewPlayer(ClientId clientId);

private:
	btScalar mass_;
	btMotionState* motionState_;
	btCollisionShape* collisionShape_;
};
