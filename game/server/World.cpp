#include "World.h"


World::World(btCollisionDispatcher* collisionDispatcher,
	btBroadphaseInterface* overlappingPairCache,
	btConstraintSolver* solver,
	btCollisionConfiguration* collisionConfiguration) : 
btDiscreteDynamicsWorld(collisionDispatcher, overlappingPairCache, solver, collisionConfiguration),
lowestPoint_(-50) {
}

World::~World() {
}


World* World::createNewWorld() {
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new	btCollisionDispatcher(collisionConfiguration);
	btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
	return new World(dispatcher, overlappingPairCache, solver, collisionConfiguration);
}

World* World::loadNewWorld(std::string filepath) {
	if (filepath == "") {
		return createNewWorld();
	}
	else {
		//TODO implement levelLoader I'm not going to do it.
		return createNewWorld();
	}
}

bool World::isDead(Player * player) {
	
	return player->getController()->getRigidBody()->getCenterOfMassPosition().getY() <= lowestPoint_;
}

void World::spawnPlayer(Player * player) {
	//TODO Deal with more complex cases
	//Most likley set spawn zones
	player->setSpawn(player->getId(), 10, 0);
}