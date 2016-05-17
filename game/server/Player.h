#pragma once

#include "btBulletDynamicsCommon.h"
#include "Client.h"
#include "Hat.h"
#include "DynamicCharacterController.h"

#include "BulletDynamics/Character/btCharacterControllerInterface.h"

class Player {
public:
	Player(btRigidBodyConstructionInfo& info);
	Player(btRigidBodyConstructionInfo& info, unsigned int id);
	Player(btRigidBodyConstructionInfo& info, unsigned int id, unsigned int lives);
	Player(btCollisionObject* body);
	~Player();
	static Player* createNewPlayer(ClientId clientId, btCollisionShape* collisionShape);
	int getId();
	void setLives(unsigned int lives);
	unsigned int getLives();
	void setSpawn(int x, int y, int z);
	Hat*  setHat(Hat * hat);
	Hat * getHat() { return currHat_; };
	int getHatType();
	void setProjectile(btRigidBody * body, unsigned int baseVelocity);
private:

	DynamicCharacterController* getController();

	Hat * currHat_;
	const unsigned int id_;
	unsigned int lives_;
	btScalar mass_;
	btMotionState* motionState_;
	btCollisionShape* collisionShape_;
private:
	DynamicCharacterController* controller_;
};
