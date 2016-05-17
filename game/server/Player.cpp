#include "Player.h"

Player::Player(btRigidBodyConstructionInfo& info) : Player(info,0) {
	
}


//TODO SETUP LIVES TO BE IN CONFIG
Player::Player(btRigidBodyConstructionInfo& info, unsigned int id) : Player(info,id,3) {

}

Player::Player(btRigidBodyConstructionInfo& info, unsigned int id, unsigned int lives) : btRigidBody(info), id_{ id }, lives_{ lives }, currHat_{ 0 } {
	setActivationState(DISABLE_DEACTIVATION);
Player::Player(btCollisionObject* body) : controller_(new DynamicCharacterController(body)) {
}


Player::~Player() {
}
int Player::getId() {
	return id_;
}
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

	this->setCenterOfMassTransform(trans);
	this->setLinearVelocity(btVector3(0, 0, 0));

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
	this->getMotionState()->getWorldTransform(xform);
	btVector3 forward = xform.getBasis()[2];
	forward.normalize();
	btTransform trans;
	trans.setIdentity();
	trans.setOrigin(this->getCenterOfMassPosition()+forward*10);
	proj->setCenterOfMassTransform(trans);
	proj->setLinearVelocity(this->getLinearVelocity() + forward*baseVelocity);

}

DynamicCharacterController* Player::getController() {
	return controller_;
}