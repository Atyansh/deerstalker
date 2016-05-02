#pragma once

#include "btBulletDynamicsCommon.h"
#include "Client.h"
#include "Player.h"

class Bullet : public btRigidBody {
public:
	Bullet(btRigidBodyConstructionInfo& info, int id, int ownerId);
	~Bullet();

	static Bullet* createNewBullet(int id, Player* player, btCollisionShape* collisionShape);
	int getId();

private:
	static const int bulletVelocity_ = 200;
	int id_;
	int ownerId_;
	btScalar mass_;
	btMotionState* motionState_;
	btCollisionShape* collisionShape_;
};
