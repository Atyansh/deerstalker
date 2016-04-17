#include "Player.h"

Player::Player(btRigidBodyConstructionInfo& info) : btRigidBody(info) {
}

Player::~Player() {
	btCollisionShape* collisionShape = this->getCollisionShape();
	btMotionState* motionState = this->getMotionState();

	delete collisionShape;
	delete motionState;
}

Player* Player::createNewPlayer(ClientId clientId) {
	btScalar mass = 1.0;
	btVector3 localInertia(0, 0, 0);
	btCollisionShape* collisionShape = new btBoxShape(btVector3(1, 1, 1));
	collisionShape->calculateLocalInertia(mass, localInertia);

	btTransform startTransform;
	startTransform.setIdentity();

	startTransform.setOrigin(btVector3(clientId, 10, 0));

	btMotionState* motionState = new btDefaultMotionState(startTransform);

	btRigidBodyConstructionInfo info(mass, motionState, collisionShape, localInertia);

	return new Player(info);
}
