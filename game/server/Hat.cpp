#include "Hat.h"

int Hat::idCounter = 0;

Hat::Hat(btRigidBodyConstructionInfo& info, HatType hatType) : btRigidBody(info), id_(idCounter++), hatType_(hatType) {
}

Hat::~Hat() {
	btCollisionShape* collisionShape = this->getCollisionShape();
	btMotionState* motionState = this->getMotionState();

	delete collisionShape;
	delete motionState;
}

Hat* Hat::createNewHat(HatType hatType, btCollisionShape* shape) {
	btScalar mass = 1.0;
	btVector3 localInertia(0, 0, 0);
	btCollisionShape* collisionShape = shape;
	collisionShape->calculateLocalInertia(mass, localInertia);

	btTransform startTransform;
	startTransform.setIdentity();

	int worldSize = 50;
	int xPosition = (rand() % worldSize) - 25;
	int yPosition = 10;
	int zPosition = (rand() % worldSize) - 25;
	startTransform.setOrigin(btVector3(xPosition, yPosition, zPosition));

	btMotionState* motionState = new btDefaultMotionState(startTransform);

	btRigidBodyConstructionInfo info(mass, motionState, collisionShape, localInertia);

	return new Hat(info, hatType);
}

HatType Hat::getHatType() {
	return hatType_;
}

int Hat::getHatId() {
	return id_;
}