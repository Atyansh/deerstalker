#include "BulletCollision/CollisionShapes/btMultiSphereShape.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"
#include "BulletCollision/CollisionDispatch/btCollisionWorld.h"
#include "LinearMath/btDefaultMotionState.h"
#include "DynamicCharacterController.h"

DynamicCharacterController::DynamicCharacterController(btCollisionObject* body) {
	m_rayLambda = 1.0;
	m_rayLambda = 1.0;
	m_shape = body->getCollisionShape();
	m_rigidBody = NULL;
	setup(100.0, 1.0, 20.0);
}

DynamicCharacterController::~DynamicCharacterController() {
}

void DynamicCharacterController::setup(btScalar height, btScalar width, btScalar stepHeight) {
	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(btVector3(0.0, 10.0, 0.0));
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo cInfo(1.0, myMotionState, m_shape);
	m_rigidBody = new btRigidBody(cInfo);
	
	m_rigidBody->setSleepingThresholds(0.0, 0.0);
	m_rigidBody->setAngularFactor(0.0);

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
	btVector3 forward = xform.getBasis()[2];
	down.normalize();
	forward.normalize();

	m_raySource = xform.getOrigin() + btVector3(0.0, 0.1, 0.0);

	m_rayTarget = m_raySource + down * btScalar(1.1);

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

	rayCallback.m_closestHitFraction = 1.0;
	collisionWorld->rayTest(m_raySource, m_rayTarget, rayCallback);

	if (rayCallback.hasHit()) {
		m_rayLambda = rayCallback.m_closestHitFraction;
	}
	else {
		m_rayLambda = 1.0;
	}
	/*
	btVector3 linearVelocity = m_rigidBody->getLinearVelocity();
	linearVelocity *= btVector3(0.2, 1, 0.2);
	m_rigidBody->setLinearVelocity(linearVelocity);
	*/
}

void DynamicCharacterController::playerStep(const btCollisionWorld*, btVector3& dir) {
	/*
	btTransform xform;
	m_rigidBody->getMotionState()->getWorldTransform(xform);

	btVector3 linearVelocity = m_rigidBody->getLinearVelocity();
	btScalar walkSpeed = 30;

	setLookDirection(m_rigidBody, dir);

	btVector3 velocity = dir * walkSpeed;
	velocity.setY(m_rigidBody->getLinearVelocity().getY());
	m_rigidBody->setLinearVelocity(velocity);

	m_rigidBody->getMotionState()->setWorldTransform(xform);
	*/

	setLookDirection(m_rigidBody, dir);
	dir.setY(0);
	m_rigidBody->applyCentralForce(dir * 10);
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
	return m_rayLambda < btScalar(1.0);
}

void DynamicCharacterController::warp(const btVector3& origin) {
}

void DynamicCharacterController::registerPairCacheAndDispatcher(btOverlappingPairCache* pairCache, btCollisionDispatcher* dispatcher) {

}

void DynamicCharacterController::setLookDirection(btRigidBody* body, const btVector3& newLook) {
	btVector3 localLook(0.0f, 0.0f, 1.0f);
	btVector3 rotationAxis(0.0f, 1.0f, 0.0f);

	// compute currentLook and angle
	btTransform transform = body->getCenterOfMassTransform();
	btQuaternion rotation = transform.getRotation();
	btVector3 currentLook = quatRotate(rotation, localLook);
	btScalar angle = currentLook.angle(newLook);

	// compute new rotation
	btQuaternion deltaRotation(rotationAxis, angle);
	btQuaternion newRotation = deltaRotation * rotation;

	// apply new rotation
	transform.setRotation(newRotation);
	body->setCenterOfMassTransform(transform);
}