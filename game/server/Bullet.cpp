#include "Bullet.h"

Bullet::Bullet(btRigidBodyConstructionInfo& info, int id, ClientId clientId) 
	: btRigidBody(info), id_(id), clientId_(clientId) {
	setActivationState(DISABLE_DEACTIVATION);
}

Bullet::~Bullet() {
	btCollisionShape* collisionShape = this->getCollisionShape();
	btMotionState* motionState = this->getMotionState();

	delete collisionShape;
	delete motionState;
}

Bullet* Bullet::createNewBullet(int id, ClientId clientId, Player* player, btCollisionShape* collisionShape) {
	btScalar mass = 1.0;
	btVector3 localInertia(0, 0, 0);
	//btCollisionShape* collisionShape = new btSphereShape(btScalar(1.));
	collisionShape->calculateLocalInertia(mass, localInertia);

	btTransform startTransform;
	startTransform.setIdentity();

	btVector3 centerOfMassPosition = player->getCenterOfMassPosition();
	btVector3 direction = player->getOrientation().getAxis().normalize();

	startTransform.setOrigin(centerOfMassPosition + (direction * 2));

	btMotionState* motionState = new btDefaultMotionState(startTransform);

	btRigidBodyConstructionInfo info(mass, motionState, collisionShape, localInertia);

	Bullet* newBullet = new Bullet(info, id, clientId);
	
	newBullet->setLinearVelocity(player->getLinearVelocity() + direction * bulletVelocity_);

	return newBullet;
}

ClientId Bullet::getClientId() {
	return clientId_;
}

int Bullet::getId() {
	return id_;
}