#pragma once

#include "btBulletDynamicsCommon.h"
#include "Client.h"

class Player : public btRigidBody {
public:
	Player(btRigidBodyConstructionInfo& info, int id);
	~Player();

	static Player* createNewPlayer(ClientId clientId, int id, btCollisionShape* collisionShape);
	int getId();
	btVector3 getUpAxis();
	btVector3 getLookAt();
	btVector3 getSideAxis();
	void setLookAt(btVector3 * lookAt);

private:
	int id_;
	btVector3 upAxis_;
	btVector3 lookAt_;
	btScalar mass_;
	btMotionState* motionState_;
	btCollisionShape* collisionShape_;
};
