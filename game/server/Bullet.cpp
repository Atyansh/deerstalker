#include "Bullet.h"

Bullet::Bullet(btRigidBodyConstructionInfo& info, int id, int ownerId) 
	: btRigidBody(info), id_(id), ownerId_(ownerId) {
	setActivationState(DISABLE_DEACTIVATION);
}

Bullet::~Bullet() {
	btCollisionShape* collisionShape = this->getCollisionShape();
	btMotionState* motionState = this->getMotionState();

	delete collisionShape;
	delete motionState;
}

Bullet* Bullet::createNewBullet(int id, Player* player, btCollisionShape* collisionShape) {
	btScalar mass = 0.5;
	btVector3 localInertia(0, 0, 0);
	collisionShape->calculateLocalInertia(mass, localInertia);

	btTransform startTransform;
	startTransform.setIdentity();

	btVector3 centerOfMassPosition = player->getCenterOfMassPosition();
	btVector3 direction = player->getLookAt();

	startTransform.setOrigin(centerOfMassPosition + (direction * 5));

	btMotionState* motionState = new btDefaultMotionState(startTransform);

	btRigidBodyConstructionInfo info(mass, motionState, collisionShape, localInertia);

	Bullet* newBullet = new Bullet(info, id, player->getId());
	
	newBullet->setLinearVelocity(player->getLinearVelocity() + direction * bulletVelocity_);

	return newBullet;
}

int Bullet::getId() {
	return id_;
}