#include "Bullet.h"

Bullet::Bullet(btRigidBodyConstructionInfo& info, int id, int ownerId)
	: btRigidBody(info), id_(id), ownerId_(ownerId), timestamp_(duration_cast<milliseconds>(
		system_clock::now().time_since_epoch())) {
	setActivationState(DISABLE_DEACTIVATION);
	setAngularFactor(btVector3(0, 0, 0));
}

Bullet::~Bullet() {
	btCollisionShape* collisionShape = this->getCollisionShape();
	btMotionState* motionState = this->getMotionState();

	delete motionState;
}

Bullet* Bullet::createNewBullet(int id, btCollisionShape* collisionShape,int pId) {
	btScalar mass = 0.5;
	btVector3 localInertia(0, 0, 0);
	collisionShape->calculateLocalInertia(mass, localInertia);

	btTransform startTransform;
	startTransform.setIdentity();


	btMotionState* motionState = new btDefaultMotionState(startTransform);

	btRigidBodyConstructionInfo info(mass, motionState, collisionShape, localInertia);

	Bullet* newBullet = new Bullet(info, id, pId);

	return newBullet;
}

unsigned int Bullet::getId() {
	return id_;
}