#include "World.h"


World::World(btCollisionDispatcher* collisionDispatcher,
	btBroadphaseInterface* overlappingPairCache,
	btConstraintSolver* solver,
	btCollisionConfiguration* collisionConfiguration) : 
btDiscreteDynamicsWorld(collisionDispatcher, overlappingPairCache, solver, collisionConfiguration) {
}

World::~World() {
}

std::unique_ptr<World> World::createNewWorld() {
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new	btCollisionDispatcher(collisionConfiguration);
	btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
	return std::make_unique<World>(dispatcher, overlappingPairCache, solver, collisionConfiguration);
}