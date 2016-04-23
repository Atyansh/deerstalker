#pragma once

#include "btBulletDynamicsCommon.h"
#include "Client.h"

class Player : public btRigidBody {
public:
	Player(btRigidBodyConstructionInfo& info);
	~Player();

	static Player* createNewPlayer(ClientId clientId, btCollisionShape* collisionShape);

private:
	btScalar mass_;
	btMotionState* motionState_;
	btCollisionShape* collisionShape_;
};
