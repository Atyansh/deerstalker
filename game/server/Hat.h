#pragma once

#include "btBulletDynamicsCommon.h"
#include "Client.h"

class Hat : public btRigidBody {
public:
	Hat(btRigidBodyConstructionInfo& info);
	~Hat();

	static Hat* createNewHat(int hatType);
	int getHatType();
	int getHatId();

	const static int WIZARD_HAT = 1;
	const static int BASEBALL_HAT = 2;

	static int idCounter;

	int playerId_;

private:
	btScalar mass_;
	btMotionState* motionState_;
	btCollisionShape* collisionShape_;
	int hatType_;
	int id_;
};