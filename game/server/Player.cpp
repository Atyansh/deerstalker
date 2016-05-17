#include "Player.h"

Player::Player(btCollisionObject* body, uint32_t id, uint32_t lives) : controller_(new DynamicCharacterController(body)), id_(id), lives_(lives), currHat_(nullptr) {
}

int Player::getId() {
	return id_;
}

/*
Player* Player::createNewPlayer(ClientId clientId, btCollisionShape* collisionShape) {
	btScalar mass = 1.0;
	btVector3 localInertia(0, 0, 0);
	//btCollisionShape* collisionShape = new btBoxShape(btVector3(1, 1, 1));
	collisionShape->calculateLocalInertia(mass, localInertia);

	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(btVector3(clientId, 10, 0));

	btMotionState* motionState = new btDefaultMotionState(startTransform);

	btRigidBodyConstructionInfo info(mass, motionState, collisionShape, localInertia);
	
	return new Player(info,clientId);
}
*/

void Player::setLives(unsigned int lives) {
	lives_ = lives;
}

unsigned int Player::getLives() {
	return lives_;
}
void Player::setSpawn(int x, int y, int z) {
	btTransform trans;
	trans.setIdentity();
	trans.setOrigin(btVector3(x, y, z));

	this->getController()->getRigidBody()->setCenterOfMassTransform(trans);
	this->getController()->getRigidBody()->setLinearVelocity(btVector3(0, 0, 0));
}

Hat * Player::setHat(Hat * hat) {
	Hat * oHat = currHat_;
	currHat_ = hat;
	return oHat;
}
int Player::getHatType() {
	return currHat_ == 0 ? -1 : currHat_->getHatType();
}

void Player::setProjectile(btRigidBody * proj,unsigned int baseVelocity) {
	btTransform xform;
	this->getController()->getRigidBody()->getMotionState()->getWorldTransform(xform);
	btVector3 forward = xform.getBasis()[2];
	forward.normalize();
	btTransform trans;
	trans.setIdentity();
	trans.setOrigin(this->getController()->getRigidBody()->getCenterOfMassPosition() + forward*10 + btVector3(0, 2, 0));
	proj->setCenterOfMassTransform(trans);
	proj->setLinearVelocity(this->getController()->getRigidBody()->getLinearVelocity() + forward*baseVelocity);
}

DynamicCharacterController* Player::getController() {
	return controller_;
}