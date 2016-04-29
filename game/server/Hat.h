#pragma once

#include "btBulletDynamicsCommon.h"
#include "Client.h"

class Hat : public btRigidBody {
public:
	Hat(btRigidBodyConstructionInfo& info);
	~Hat();

	static Hat* createNewHat(ClientId clientId, int hatType);
	int getHatType();

	const static int WIZARD_HAT = 0;
	const static int BASEBALL_HAT = 1;

private:
	btScalar mass_;
	btMotionState* motionState_;
	btCollisionShape* collisionShape_;
	int hatType_;
};