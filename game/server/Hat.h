#pragma once

#include "btBulletDynamicsCommon.h"
#include "Client.h"

#include "util\HatType.h"

class Hat : public btRigidBody {
public:
	Hat(btRigidBodyConstructionInfo& info, HatType hatTypee);
	~Hat();

	static Hat* createNewHat(HatType hatType, btCollisionShape* shape);
	HatType getHatType();
	int getHatId();

	static int idCounter;

	int playerId_;

private:
	btScalar mass_;
	btMotionState* motionState_;
	btCollisionShape* collisionShape_;
	HatType hatType_;
	int id_;
};