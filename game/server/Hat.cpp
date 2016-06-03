#include "Hat.h"

int Hat::idCounter = 0;

const milliseconds Hat::HAT_LIFESPAN = milliseconds(120000);

Hat::Hat(btRigidBodyConstructionInfo& info, HatType hatType) : btRigidBody(info), id_(idCounter++), hatType_(hatType), timestamp_(duration_cast<milliseconds>(system_clock::now().time_since_epoch())) {
}

Hat::~Hat() {
	btMotionState* motionState = this->getMotionState();

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
	int xPosition = (rand() % (HIGH_X - LOW_X)) + LOW_X;
	int zPosition = (rand() % (HIGH_Z - LOW_Z)) + LOW_Z;
	startTransform.setOrigin(btVector3(xPosition, Y, zPosition));

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