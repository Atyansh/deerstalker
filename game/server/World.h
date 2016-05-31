#pragma once

#include <unordered_map>
#include "Client.h"
#include "btBulletDynamicsCommon.h"
#include "Player.h"
#include <iostream>

class World : public btDiscreteDynamicsWorld {
public:
	World::World(btCollisionDispatcher* collisionDispatcher,
		btBroadphaseInterface* overlappingPairCache,
		btConstraintSolver* solver,
		btCollisionConfiguration* collisionConfiguration);
	~World();

	static World* createNewWorld();
	bool isDead(Player * p);
	void spawnPlayer(Player *p);
	
private:
	float lowestPoint_;
};