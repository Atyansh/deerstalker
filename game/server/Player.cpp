#include "Player.h"

const btVector3 Player::P1_SPAWN_POINT = btVector3(-60, 130, -60);
const btVector3 Player::P2_SPAWN_POINT = btVector3(-55, 120, 70);
const btVector3 Player::P3_SPAWN_POINT = btVector3(55, 110, 25);
const btVector3 Player::P4_SPAWN_POINT = btVector3(80, 100, -40);

Player::Player(btCollisionObject* body, uint32_t id, uint32_t lives) : controller_(new DynamicCharacterController(body)), id_(id), lives_(lives), currHat_(nullptr) {
	setSpawn();
}

Player::~Player() {
	delete controller_;
}

int Player::getId() {
	return id_;
}

void Player::setLives(unsigned int lives) {
	lives_ = lives;
}

unsigned int Player::getLives() {
	return lives_;
}
void Player::setSpawn() {
	btTransform trans;
	trans.setIdentity();

	switch (id_) {
	case 1:
		trans.setOrigin(P1_SPAWN_POINT);
		break;
	case 2:
		trans.setOrigin(P2_SPAWN_POINT);
		break;
	case 3:
		trans.setOrigin(P3_SPAWN_POINT);
		break;
	case 4:
		trans.setOrigin(P4_SPAWN_POINT);
		break;
	default:
		std::cerr << "ERROR: Invalid Player Id: " << id_ << std::endl;
	}
	this->getController()->getRigidBody()->setCenterOfMassTransform(trans);
	this->getController()->getRigidBody()->setLinearVelocity(btVector3(0, 0, 0));
}

Hat * Player::setHat(Hat * hat) {
	Hat * oHat = currHat_;
	currHat_ = hat;
	return oHat;
}

void Player::setProjectile(btRigidBody * proj, unsigned int baseVelocity) {
	btTransform transform = this->getController()->getRigidBody()->getCenterOfMassTransform();
	btVector3 localLook(0.0f, 0.0f, 1.0f);
	btQuaternion rotation = transform.getRotation();
	btVector3 currentLook = quatRotate(rotation, localLook);

	btTransform trans;
	trans.setIdentity();
	trans.setOrigin(this->getController()->getRigidBody()->getCenterOfMassPosition() + currentLook*10 + btVector3(0, 5, 0));
	proj->setCenterOfMassTransform(trans);
	proj->setLinearVelocity(this->getController()->getRigidBody()->getLinearVelocity() + currentLook*baseVelocity);
}

DynamicCharacterController* Player::getController() {
	return controller_;
}