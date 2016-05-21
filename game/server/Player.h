#pragma once

#include "btBulletDynamicsCommon.h"
#include "Client.h"
#include "Hat.h"
#include "DynamicCharacterController.h"

#include "BulletDynamics/Character/btCharacterControllerInterface.h"

class Player {
public:
	Player(btCollisionObject* body, uint32_t id, uint32_t lives);
	~Player() {}

	//static Player* createNewPlayer(ClientId clientId, btCollisionShape* collisionShape);

	int getId();
	void setLives(unsigned int lives);
	unsigned int getLives();


	Hat*  setHat(Hat * hat);
	Hat * getHat() { return currHat_; };

	void setSpawn(int x, int y, int z);
	void setProjectile(btRigidBody * body, unsigned int baseVelocity);

	DynamicCharacterController* getController();

private:
	Hat * currHat_;
	const unsigned int id_;
	unsigned int lives_;
private:
	DynamicCharacterController* controller_;
};
