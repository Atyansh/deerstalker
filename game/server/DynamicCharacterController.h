#ifndef CHARACTER_CONTROLLER_H
#define CHARACTER_CONTROLLER_H

#include <iostream>

#include "LinearMath/btVector3.h"

#include "BulletDynamics/Character/btCharacterControllerInterface.h"

class btCollisionShape;
class btRigidBody;
class btCollisionWorld;

///DynamicCharacterController is obsolete/unsupported at the moment
class DynamicCharacterController : public btCharacterControllerInterface
{
protected:
	btCollisionShape* m_shape;
	btRigidBody* m_rigidBody;

	btVector3 downRaySource;
	btVector3 downRayTarget;
	btScalar downRayLambda;

	btVector3 forwardRaySource;
	btVector3 forwardRayTarget;
	btScalar forwardRayLambda;

	float range = 5;

	const btCollisionObject* punchTarget;

public:
	DynamicCharacterController(btCollisionObject* body);
	~DynamicCharacterController();
	void setup(btScalar height = 2.0, btScalar width = 0.25, btScalar stepHeight = 0.25);
	void destroy();

	btCollisionObject* getCollisionObject();

	void preStep(btCollisionWorld* collisionWorld);
	void playerStep(const btCollisionWorld* collisionWorld, btVector3& dir);
	bool canJump() const;
	void jump();

	bool onGround() const;

	// TODO:
	void warp(const btVector3& origin);
	void registerPairCacheAndDispatcher(btOverlappingPairCache* pairCache, btCollisionDispatcher* dispatcher);

	void playerStep(btCollisionWorld* collisionWorld, btScalar dt) {}
	void reset(btCollisionWorld *collisionWorld) {}
	void setWalkDirection(const btVector3 &walkDirection) {}
	void setVelocityForTimeInterval(const btVector3 &velocity, btScalar timeInterval) {}
	void setUpInterpolate(bool value) {}
	
	void updateAction(btCollisionWorld *collisionWorld, btScalar deltaTimeStep) {
		preStep(collisionWorld);
	}
	void debugDraw(btIDebugDraw *debugDrawer) {}

	btRigidBody* getRigidBody() {
		return m_rigidBody;
	}

	btCollisionObject* getPunchTarget() {
		return (btCollisionObject*)punchTarget;
	}

	void setRange(float range) {
		this->range = range;
	}

	float getRange() {
		return range;
	}

	void setLookDirection(const btVector3& newLook);
};

#endif
