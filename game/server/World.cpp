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

bool World::isDead(Player * player) {
	return player->getCenterOfMassPosition().getY() <= lowestPoint_;
}

void World::spawnPlayer(Player * player) {
	player->setSpawn();
}