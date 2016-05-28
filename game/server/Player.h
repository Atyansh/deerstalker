#pragma once

#include "btBulletDynamicsCommon.h"
#include "Client.h"
#include "Hat.h"
#include "DynamicCharacterController.h"

#include "BulletDynamics/Character/btCharacterControllerInterface.h"

class Player : public btRigidBody {
public:
	Player(btRigidBodyConstructionInfo& info, uint32_t id, uint32_t lives);
	~Player();

	static Player* createNewPlayer(ClientId clientId, btCollisionShape* collisionShape);

	int getId();
	void setLives(unsigned int lives);
	unsigned int getLives();


	Hat*  setHat(Hat * hat);
	Hat* getHat() {
		return currHat_;
	}

	HatType getHatType() {
		return getHat() == nullptr ? NO_HAT : getHat()->getHatType();
	}

	int32_t getHealth() {
		return health_;
	}

	bool getStunned() {
		return stunned_;
	}

	void stun() {
		stunned_ = true;
	}

	void revive() {
		stunned_ = false;
	}

	void changeHealth(int delta) {
		int newHealth = health_ + delta;
		if (newHealth > 100) {
			setHealth(100);
		}
		else if (newHealth < 0) {
			setHealth(0);
		}
		else {
			setHealth(newHealth);
		}
	}

	void setSpawn();
	void setProjectile(btRigidBody * body, unsigned int baseVelocity);

	DynamicCharacterController* getController();

	static const uint32_t P1_ID = 1;
	static const uint32_t P2_ID = 2;
	static const uint32_t P3_ID = 3;
	static const uint32_t P4_ID = 4;

private:
	const unsigned int id_;
	unsigned int lives_;

	bool stunned_ = false;

	int32_t health_ = 100;

	Hat * currHat_;
	DynamicCharacterController* controller_;

	static const btVector3 P1_SPAWN_POINT;
	static const btVector3 P2_SPAWN_POINT;
	static const btVector3 P3_SPAWN_POINT;
	static const btVector3 P4_SPAWN_POINT;

	void setHealth(int32_t health) {
		health_ = health;
	}
};
