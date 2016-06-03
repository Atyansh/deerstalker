#include "Player.h"

const btVector3 Player::P1_SPAWN_POINT = btVector3(-126, 530, -50);
const btVector3 Player::P2_SPAWN_POINT = btVector3(-2, 390, 330);
const btVector3 Player::P3_SPAWN_POINT = btVector3(330, 360, -300);
const btVector3 Player::P4_SPAWN_POINT = btVector3(50, 340, -350);

Player::Player(btRigidBodyConstructionInfo& info, uint32_t id, uint32_t lives) : btRigidBody(info), id_(id), lives_(lives), currHat_(nullptr) {
	this->setSleepingThresholds(0.0, 0.0);
	this->setAngularFactor(0.0);
	controller_ = new DynamicCharacterController(this);
	setSpawn();
}

Player::~Player() {
	delete this->getMotionState();
	delete controller_;
}

Player* Player::createNewPlayer(ClientId clientId, btCollisionShape* collisionShape) {
	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(btVector3(0.0, 50.0, 0.0));
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo cInfo(1.0, myMotionState, collisionShape);
	return new Player(cInfo, clientId, 3);
}

int Player::getId() {
	return id_;
}

void Player::setLives(uint32_t lives) {
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
	this->setCenterOfMassTransform(trans);
	this->getMotionState()->setWorldTransform(trans);
	this->setLinearVelocity(btVector3(0, 0, 0));
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
	trans.setOrigin(this->getController()->getRigidBody()->getCenterOfMassPosition() +
		            (currentLook * 7) + btVector3(0, 5, 0));
	trans.setRotation(rotation);
	proj->setCenterOfMassTransform(trans);
	proj->setLinearVelocity(this->getController()->getRigidBody()->getLinearVelocity() +
		                    currentLook * baseVelocity);
}

DynamicCharacterController* Player::getController() {
	return controller_;
}