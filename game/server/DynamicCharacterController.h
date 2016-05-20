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
	btScalar m_halfHeight;
	btCollisionShape* m_shape;
	btRigidBody* m_rigidBody;

	btVector3 m_raySource[2];
	btVector3 m_rayTarget[2];
	btScalar m_rayLambda[2];
	btVector3 m_rayNormal[2];

	btScalar m_turnAngle;

	btScalar m_maxLinearVelocity;
	btScalar m_walkVelocity;
	btScalar m_turnVelocity;
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

	void setLookDirection(btRigidBody* body, const btVector3& newLook);
};

#endif
