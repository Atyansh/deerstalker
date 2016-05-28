#include "BulletCollision/CollisionShapes/btMultiSphereShape.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"
#include "BulletCollision/CollisionDispatch/btCollisionWorld.h"
#include "LinearMath/btDefaultMotionState.h"
#include "DynamicCharacterController.h"

DynamicCharacterController::DynamicCharacterController(btCollisionObject* body) {
	downRayLambda = 1.0;
	forwardRayLambda = 1.0;
	m_rigidBody = (btRigidBody*) body;
}

DynamicCharacterController::~DynamicCharacterController() {
}

void DynamicCharacterController::destroy() {
	if (m_shape) {
		delete m_shape;
	}

	if (m_rigidBody) {
		delete m_rigidBody;
		m_rigidBody = 0;
	}
}

btCollisionObject* DynamicCharacterController::getCollisionObject() {
	return m_rigidBody;
}

void DynamicCharacterController::preStep(btCollisionWorld* collisionWorld) {
	btTransform xform;
	m_rigidBody->getMotionState()->getWorldTransform(xform);
	btVector3 down = -xform.getBasis()[1];
	down.normalize();

	class ClosestNotMe : public btCollisionWorld::ClosestRayResultCallback {
	public:
		ClosestNotMe(btRigidBody* me) : btCollisionWorld::ClosestRayResultCallback(btVector3(0.0, 0.0, 0.0), btVector3(0.0, 0.0, 0.0)) {
			m_me = me;
		}

		virtual btScalar addSingleResult(btCollisionWorld::LocalRayResult& rayResult, bool normalInWorldSpace) {
			if (rayResult.m_collisionObject == m_me)
				return 1.0;

			return ClosestRayResultCallback::addSingleResult(rayResult, normalInWorldSpace
			);
		}
	protected:
		btRigidBody* m_me;
	};

	ClosestNotMe rayCallback(m_rigidBody);

	downRaySource = xform.getOrigin() + btVector3(0.0, 0.1, 0.0);
	downRayTarget = downRaySource + down * btScalar(1.1);

	rayCallback.m_closestHitFraction = 1.0;
	collisionWorld->rayTest(downRaySource, downRayTarget, rayCallback);

	if (rayCallback.hasHit()) {
		downRayLambda = rayCallback.m_closestHitFraction;
	}
	else {
		downRayLambda = 1.0;
	}

	ClosestNotMe rayCallback2(m_rigidBody);

	btVector3 localLook(0.0f, 0.0f, 1.0f);
	btTransform transform = m_rigidBody->getCenterOfMassTransform();
	btQuaternion rotation = transform.getRotation();
	btVector3 currentLook = quatRotate(rotation, localLook);
	
	forwardRaySource = xform.getOrigin() + btVector3(0, 5, 0);
	forwardRayTarget = forwardRaySource + currentLook * range;

	rayCallback2.m_closestHitFraction = 1.0;
	collisionWorld->rayTest(forwardRaySource, forwardRayTarget, rayCallback2);

	if (rayCallback2.hasHit()) {
		punchTarget = rayCallback2.m_collisionObject;
	}
	else {
		forwardRayLambda = 1.0;
		punchTarget = nullptr;
	}
}

void DynamicCharacterController::playerStep(const btCollisionWorld*, btVector3& dir) {
	setLookDirection(dir);
	dir.setY(0);
	m_rigidBody->applyCentralForce(dir * 5);
}

bool DynamicCharacterController::canJump() const {
	return onGround();
}

void DynamicCharacterController::jump() {
	if (!canJump()) {
		return;
	}

	btTransform xform;
	m_rigidBody->getMotionState()->getWorldTransform(xform);
	btVector3 up = xform.getBasis()[1];
	up.normalize();
	btScalar magnitude = 15;
	m_rigidBody->applyCentralImpulse(up * magnitude);
}

bool DynamicCharacterController::onGround() const {
	return downRayLambda < btScalar(1.0);
}

void DynamicCharacterController::warp(const btVector3& origin) {
}

void DynamicCharacterController::registerPairCacheAndDispatcher(btOverlappingPairCache* pairCache, btCollisionDispatcher* dispatcher) {

}

void DynamicCharacterController::setLookDirection(const btVector3& newLook) {
	btVector3 localLook(0.0f, 0.0f, 1.0f);
	btVector3 rotationAxis(0.0f, 1.0f, 0.0f);
	
	// compute currentLook and angle
	btTransform transform = m_rigidBody->getCenterOfMassTransform();
	btQuaternion rotation = transform.getRotation();
	btVector3 currentLook = quatRotate(rotation, localLook);
	btScalar angle = currentLook.angle(newLook.normalized());

	// compute new rotation
	btQuaternion deltaRotation(rotationAxis, angle);
	btQuaternion newRotation = deltaRotation * rotation;

	// apply new rotation
	transform.setRotation(newRotation);
	m_rigidBody->setCenterOfMassTransform(transform);
}