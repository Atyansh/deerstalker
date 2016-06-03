#pragma once

#include "btBulletDynamicsCommon.h"
#include "Game.h"

class Game;

class GravityController : public btActionInterface {
public:
	GravityController(Game* game);
	~GravityController();

	void updateAction(btCollisionWorld *collisionWorld, btScalar deltaTimeStep);

	void debugDraw(btIDebugDraw *debugDrawer) {
	}

	void activate() {
		active = true;
	};

	void deactivate() {
		active = false;
	};

	bool getActive() {
		return active;
	}

private:
	Game* game_;
	bool active = false;
};