#include "World.h"


World::World(btCollisionDispatcher* collisionDispatcher,
	btBroadphaseInterface* overlappingPairCache,
	btConstraintSolver* solver,
	btCollisionConfiguration* collisionConfiguration) : 
btDiscreteDynamicsWorld(collisionDispatcher, overlappingPairCache, solver, collisionConfiguration) {
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
		return generateDefaultWorld();
	}
	else {
		//TODO implement levelLoader I'm not going to do it.
		return generateDefaultWorld();
	}
}

World* World::generateDefaultWorld() {
	World * world = createNewWorld();
	btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(50.), btScalar(50.), btScalar(50.)));

	//btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 5);
	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(0, -56, 0));

	{
		btScalar mass(0.);

		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			groundShape->calculateLocalInertia(mass, localInertia);

		btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
		btRigidBody* body = new btRigidBody(rbInfo);

		world->addRigidBody(body);
	}
	world->lowestPoint_ = -56;
	return world;
}

bool World::isDead(Player * player) {
	std::cout << player->getCenterOfMassPosition().getY() << std::endl;
	return player->getCenterOfMassPosition().getY() <= lowestPoint_;
}

void World::spawnPlayer(Player * player) {
	//TODO Deal with more complex cases
	btTransform trans;
	trans.setIdentity();
	trans.setOrigin(btVector3(player->getId(), 10, 0));

	player->setCenterOfMassTransform(trans);
	player->setLinearVelocity(btVector3(0, 0, 0));
}