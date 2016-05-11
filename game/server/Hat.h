#pragma once

#include <ctime>
#include "btBulletDynamicsCommon.h"
#include "Client.h"

class Hat : public btRigidBody {
public:
	Hat(btRigidBodyConstructionInfo& info);
	~Hat();

	static Hat* createNewHat(int hatType);
	int getHatType();
	int getHatId();
	bool isLive() { return isLive_; }
	void setStatus(bool newStatus) { isLive_ = newStatus; }
	int getDuration();

	const static int WIZARD_HAT = 0;
	const static int BASEBALL_HAT = 1;

	static int idCounter;

private:
	btScalar mass_;
	btMotionState* motionState_;
	btCollisionShape* collisionShape_;
	int hatType_;
	int id_;
	bool isLive_;
	std::clock_t spawnTime_;
};