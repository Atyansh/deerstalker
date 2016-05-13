#pragma once

#include "btBulletDynamicsCommon.h"
#include "Client.h"

class Player : public btRigidBody {
public:
	Player(btRigidBodyConstructionInfo& info);
	Player(btRigidBodyConstructionInfo& info,int id);
	~Player();
	static Player* createNewPlayer(ClientId clientId, btCollisionShape* collisionShape);
	int getId();
private:
	const int id_;
	btScalar mass_;
	btMotionState* motionState_;
	btCollisionShape* collisionShape_;
};
