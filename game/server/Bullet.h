#pragma once
#include "btBulletDynamicsCommon.h"
#include "Client.h"
#include "Player.h"

#include <chrono>

using namespace std::chrono;

class Bullet : public btRigidBody {
public:
	Bullet(btRigidBodyConstructionInfo& info, int id, int ownerId);
	~Bullet();

	static Bullet* createNewBullet(int id,btCollisionShape* collisionShape,int pid);
	unsigned int getId();
	unsigned int getVelocity() { return bulletVelocity_; }

	milliseconds getTimestamp() {
		return timestamp_;
	}

private:
	const unsigned int bulletVelocity_ = 200;
	int id_;
	int ownerId_;
	btScalar mass_;
	btMotionState* motionState_;
	btCollisionShape* collisionShape_;
	milliseconds timestamp_;
};