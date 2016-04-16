#pragma once

#include <unordered_map>
#include "Client.h"
#include "btBulletDynamicsCommon.h"


class World : public btDiscreteDynamicsWorld {
public:
	World::World(btCollisionDispatcher* collisionDispatcher,
		btBroadphaseInterface* overlappingPairCache,
		btConstraintSolver* solver,
		btCollisionConfiguration* collisionConfiguration);
	~World();

	static std::unique_ptr<World> createNewWorld();

private:
};