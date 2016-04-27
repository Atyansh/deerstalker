#pragma once

#include "btBulletDynamicsCommon.h"
#include "Client.h"
#include "Player.h"

class Bullet : public btRigidBody {
public:
	Bullet(btRigidBodyConstructionInfo& info, int id, ClientId clientId);
	~Bullet();

	static Bullet* createNewBullet(int id, ClientId clientId, Player* player, btCollisionShape* collisionShape);
	ClientId getClientId();
	int getId();

private:
	int id_;
	ClientId clientId_;
	btScalar mass_;
	btMotionState* motionState_;
	btCollisionShape* collisionShape_;
};
