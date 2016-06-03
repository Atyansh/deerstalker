#pragma once

#include "btBulletDynamicsCommon.h"
#include "Client.h"
#include "Hat.h"
#include "DynamicCharacterController.h"

#include "BulletDynamics/Character/btCharacterControllerInterface.h"

#include <chrono>

using namespace std::chrono;

class DynamicCharacterController;

class Player : public btRigidBody {
public:
	Player(btRigidBodyConstructionInfo& info, uint32_t id, uint32_t lives);
	~Player();

	static Player* createNewPlayer(ClientId clientId, btCollisionShape* collisionShape);

	int getId();
	void setLives(uint32_t lives);
	uint32_t getLives();


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

	void setHealth(int32_t health) {
		health_ = health;
	}

	bool getStunned() {
		return stunned_;
	}

	void setStunned(bool stunned) {
		stunned_ = stunned;
	}

	void setVisible(bool visible) {
		visible_ = visible;
	}

	bool getVisible() {
		return visible_;
	}

	Player* getMyGrabber() {
		return myGrabber_;
	}

	void setMyGrabber(Player* myGrabber) {
		myGrabber_ = myGrabber;
	}

	Player* getGrabbedPlayer() {
		return grabbedPlayer_;
	}

	void setGrabbedPlayer(Player* grabbedPlayer) {
		grabbedPlayer_ = grabbedPlayer;
	}

	milliseconds getStunTimestamp() {
		return stunTimestamp_;
	}

	void setStunTimestamp(milliseconds timestamp) {
		stunTimestamp_ = timestamp;
	}

	bool getDead() {
		return dead_;
	}

	void setDead(bool dead) {
		dead_ = dead;
	}

	milliseconds getDeadTimestamp() {
		return deadTimestamp_;
	}

	void setDeadTimestamp(milliseconds timestamp) {
		deadTimestamp_ = timestamp;
	}

	bool getReady() {
		return ready_;
	}

	void setReady(bool ready) {
		ready_ = ready;
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
	uint32_t lives_;

	milliseconds stunTimestamp_;
	milliseconds deadTimestamp_;

	bool stunned_ = false;
	bool visible_ = true;

	bool dead_ = false;
	bool ready_ = false;

	Player* myGrabber_ = nullptr;
	Player* grabbedPlayer_ = nullptr;

	int32_t health_ = 100;

	Hat * currHat_;

	DynamicCharacterController* controller_;

	static const btVector3 P1_SPAWN_POINT;
	static const btVector3 P2_SPAWN_POINT;
	static const btVector3 P3_SPAWN_POINT;
	static const btVector3 P4_SPAWN_POINT;
};
