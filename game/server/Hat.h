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

	static const int LOW_X = -50;
	static const int HIGH_X = 230;

	static const int LOW_Z = -130;
	static const int HIGH_Z = 150;

	static const int Y = 355;
};