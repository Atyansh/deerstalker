#include "Hat.h"

Hat::Hat(btRigidBodyConstructionInfo& info) : btRigidBody(info) {
	setActivationState(DISABLE_DEACTIVATION);
}

Hat::~Hat() {
	btCollisionShape* collisionShape = this->getCollisionShape();
	btMotionState* motionState = this->getMotionState();

	delete collisionShape;
	delete motionState;
}

Hat* Hat::createNewHat(int hatType) {
	btScalar mass = 1.0;
	btVector3 localInertia(0, 0, 0);
	btCollisionShape* collisionShape = new btBoxShape(btVector3(1, 1, 1));
	collisionShape->calculateLocalInertia(mass, localInertia);

	btTransform startTransform;
	startTransform.setIdentity();

	int worldSize = 21;
	int xPosition = rand() % worldSize - 10;
	int yPosition = 10;
	int zPosition = rand() % worldSize - 10;
	startTransform.setOrigin(btVector3(xPosition, yPosition, zPosition));

	btMotionState* motionState = new btDefaultMotionState(startTransform);

	btRigidBodyConstructionInfo info(mass, motionState, collisionShape, localInertia);

	return new Hat(info);
}

int Hat::getHatType() {
	return hatType_;
}

int Hat::getHatId() {
	return id_;
}