#include "Player.h"

Player::Player(btRigidBodyConstructionInfo& info, int id) 
	: btRigidBody(info), id_(id), upAxis_(btVector3(0, 0, 1)), lookAt_(btVector3(1, 0, 0)) {
	setActivationState(DISABLE_DEACTIVATION);
}

Player::~Player() {
	btCollisionShape* collisionShape = this->getCollisionShape();
	btMotionState* motionState = this->getMotionState();

	delete collisionShape;
	delete motionState;
}

Player* Player::createNewPlayer(ClientId clientId, int id, btCollisionShape* collisionShape) {
	btScalar mass = 1.0;
	btVector3 localInertia(0, 0, 0);
	//btCollisionShape* collisionShape = new btBoxShape(btVector3(1, 1, 1));
	collisionShape->calculateLocalInertia(mass, localInertia);

	btTransform startTransform;
	startTransform.setIdentity();

	startTransform.setOrigin(btVector3(clientId, 10, 0));

	btMotionState* motionState = new btDefaultMotionState(startTransform);

	btRigidBodyConstructionInfo info(mass, motionState, collisionShape, localInertia);

	return new Player(info, id);
}

int Player::getId() {
	return id_;
}

btVector3 Player::getUpAxis() {
	return upAxis_;
}

//TODO GENERALIZE
btVector3 Player::getSideAxis() {
	return btVector3(0, 1, 0);
}

void Player::setLookAt(btVector3 * lookAt) {
	lookAt_ = *lookAt;
}

btVector3 Player::getLookAt() {
	return lookAt_;
};