#include "Hat.h"

Hat::Hat(btRigidBodyConstructionInfo& info) : btRigidBody(info) {
}

Hat::~Hat() {
	btCollisionShape* collisionShape = this->getCollisionShape();
	btMotionState* motionState = this->getMotionState();

	delete collisionShape;
	delete motionState;
}

Hat* Hat::createNewHat(ClientId clientId, int hatType) {
	btScalar mass = 1.0;
	btVector3 localInertia(0, 0, 0);
	btCollisionShape* collisionShape = new btBoxShape(btVector3(1, 1, 1));
	collisionShape->calculateLocalInertia(mass, localInertia);

	btTransform startTransform;
	startTransform.setIdentity();

	startTransform.setOrigin(btVector3(clientId, 10, 0));

	btMotionState* motionState = new btDefaultMotionState(startTransform);

	btRigidBodyConstructionInfo info(mass, motionState, collisionShape, localInertia);

	return new Hat(info);
}

int Hat::getHatType() {
	return hatType_;
}
