#include "BulletCollision/CollisionShapes/btMultiSphereShape.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"
#include "BulletCollision/CollisionDispatch/btCollisionWorld.h"
#include "LinearMath/btDefaultMotionState.h"
#include "DynamicCharacterController.h"

DynamicCharacterController::DynamicCharacterController(btCollisionObject* body) {
	downRayLambda = 1.0;
	m_rigidBody = (Player*) body;
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

void DynamicCharacterController::preStep(btCollisionWorld* collisionWorld, btScalar dt) {
	Player* myGrabber = m_rigidBody->getMyGrabber();

	if (myGrabber) {
		m_rigidBody->applyCentralImpulse(btVector3(0, 10, 0) * dt);
		btVector3 origin = m_rigidBody->getCenterOfMassPosition();
		std::cerr << origin.getX() << " " << origin.getY() << " " << origin.getZ() << std::endl;

		btTransform grabbeeTransform = m_rigidBody->getCenterOfMassTransform();
		
		btVector3 grabberPosition = myGrabber->getCenterOfMassPosition();

		grabbeeTransform.setOrigin(grabberPosition + btVector3(0, 17, 0) + grabberLook * -10);
		
		
		//setLookDirection();
		m_rigidBody->setCenterOfMassTransform(grabbeeTransform);
		origin = m_rigidBody->getCenterOfMassPosition();

		std::cerr << origin.getX() << " " << origin.getY() << " " << origin.getZ() << std::endl;

		m_rigidBody->getMotionState()->setWorldTransform(grabbeeTransform);
		return;
	}

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

	btVector3 localLook(0.0f, 0.0f, 1.0f);
	btTransform transform = m_rigidBody->getCenterOfMassTransform();
	btQuaternion rotation = transform.getRotation();
	btVector3 currentLook = quatRotate(rotation, localLook);
	btVector3 rotationAxis(0.0f, 1.0f, 0.0f);
	btScalar angle = 1.05f;

	btQuaternion rot1(rotationAxis, angle);
	btQuaternion rot2(rotationAxis, -angle);


	btQuaternion newRot1 = rot1 * rotation;
	btQuaternion newRot2 = rot2 * rotation;

	btVector3 look1 = quatRotate(newRot1, localLook);
	btVector3 look2 = quatRotate(newRot2, localLook);
	
	forwardRaySource = xform.getOrigin() + btVector3(0, 5, 0);
	forwardRayTarget = forwardRaySource + currentLook * 7;

	ClosestNotMe rayCallback2(m_rigidBody);
	rayCallback2.m_closestHitFraction = 1.0;
	collisionWorld->rayTest(forwardRaySource, forwardRayTarget, rayCallback2);

	if (rayCallback2.hasHit()) {
		punchTarget = rayCallback2.m_collisionObject;
	}
	else {
		punchTarget = nullptr;
	}

	btVector3 ram0Source = xform.getOrigin() + btVector3(0, 1, 0);
	btVector3 ram0Target = forwardRaySource + currentLook * 12;

	btVector3 ram1Source = xform.getOrigin() + btVector3(0, 1, 0);
	btVector3 ram1Target = forwardRaySource + look1 * 12;

	btVector3 ram2Source = xform.getOrigin() + btVector3(0, 1, 0);
	btVector3 ram2Target = forwardRaySource + look2 * 12;

	ClosestNotMe rayCallback3(m_rigidBody);
	rayCallback3.m_closestHitFraction = 1.0;
	ClosestNotMe rayCallback4(m_rigidBody);
	rayCallback4.m_closestHitFraction = 1.0;
	ClosestNotMe rayCallback5(m_rigidBody);
	rayCallback5.m_closestHitFraction = 1.0;

	collisionWorld->rayTest(ram0Source, ram0Target, rayCallback3);
	collisionWorld->rayTest(ram1Source, ram1Target, rayCallback4);
	collisionWorld->rayTest(ram2Source, ram2Target, rayCallback5);

	if (rayCallback3.hasHit()) {
		ramTarget = rayCallback3.m_collisionObject;
	}
	else if (rayCallback4.hasHit()){
		ramTarget = rayCallback4.m_collisionObject;
	}
	else if (rayCallback5.hasHit()){
		ramTarget = rayCallback5.m_collisionObject;
	}
	else {
		ramTarget = nullptr;
	}

	ClosestNotMe rayCallback6(m_rigidBody);
	rayCallback6.m_closestHitFraction = 1.0;

	forwardRaySource = xform.getOrigin() + btVector3(0, 5, 0);
	forwardRayTarget = forwardRaySource + currentLook * 10;

	collisionWorld->rayTest(forwardRaySource, forwardRayTarget, rayCallback6);

	if (rayCallback6.hasHit()) {
		wrenchTarget = rayCallback6.m_collisionObject;
	}
	else {
		wrenchTarget = nullptr;
	}
}

void DynamicCharacterController::playerStep(const btCollisionWorld*, btVector3& dir) {
	setLookDirection(dir);
	dir.setY(0);

	btVector3 velocity = m_rigidBody->getLinearVelocity();
	btVector3 planeVelocity = velocity;
	planeVelocity.setY(0);
	btScalar speed = planeVelocity.length();

	if (speed < 10) {
		m_rigidBody->setLinearVelocity((dir * 10) + btVector3(0, velocity.getY(), 0));
	}

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
	
	btTransform transform;
	transform = m_rigidBody->getCenterOfMassTransform();
	btQuaternion rotation = transform.getRotation();
	btVector3 currentLook = quatRotate(rotation, localLook);
	btScalar angle = currentLook.angle(newLook.normalized());

	btQuaternion deltaRotation1(rotationAxis, angle);
	btQuaternion deltaRotation2(rotationAxis, -angle);
	btQuaternion newRotation1 = deltaRotation1 * rotation;
	btQuaternion newRotation2 = deltaRotation2 * rotation;

	btVector3 testLook1 = quatRotate(newRotation1, localLook);
	btVector3 testLook2 = quatRotate(newRotation2, localLook);

	btQuaternion newRotation;

	if (testLook1.angle(newLook.normalized()) < testLook2.angle(newLook.normalized())) {
		newRotation = newRotation1;
	}
	else {
		newRotation = newRotation2;
	}

	transform.setRotation(newRotation);
	m_rigidBody->setCenterOfMassTransform(transform);
	m_rigidBody->getMotionState()->setWorldTransform(transform);
}

void DynamicCharacterController::grabOrientation(Player* grabber) {
	grabberLook = quatRotate(grabber->getCenterOfMassTransform().getRotation(), btVector3(0, 0, 1));
	setLookDirection(quatRotate(grabber->getCenterOfMassTransform().getRotation(), btVector3(0, 0, 1)));
	btVector3 localLook(0.0f, 0.0f, 1.0f);
	btVector3 rotationAxis(0.0f, 1.0f, 0.0f);

	btTransform transform;
	transform = m_rigidBody->getCenterOfMassTransform();
	btQuaternion rotation = transform.getRotation();
	
	btQuaternion deltaRotation(rotationAxis, 3.14 / 2);
	btQuaternion sideRotation = deltaRotation * rotation;
	btVector3 sideAxis = quatRotate(sideRotation, localLook);

	btQuaternion turnDelta(sideAxis, 3.14 / 2);
	btQuaternion turnRotation = turnDelta * rotation;


	transform.setRotation(turnRotation);
	m_rigidBody->setCenterOfMassTransform(transform);
	m_rigidBody->getMotionState()->setWorldTransform(transform);
}

void DynamicCharacterController::straightOrientation() {
	btVector3 localLook(0.0f, -1.0f, 0.0f);
	btVector3 rotationAxis(0.0f, 1.0f, 0.0f);

	btTransform transform;
	transform = m_rigidBody->getCenterOfMassTransform();
	btQuaternion rotation = transform.getRotation();

	btQuaternion deltaRotation(rotationAxis, 3.14 / 2);
	btQuaternion sideRotation = deltaRotation * rotation;
	btVector3 sideAxis = quatRotate(sideRotation, localLook);

	btQuaternion turnDelta(sideAxis, 3.14 / 2);
	btQuaternion turnRotation = turnDelta * rotation;

	transform.setRotation(turnRotation);
	m_rigidBody->setCenterOfMassTransform(transform);
	m_rigidBody->getMotionState()->setWorldTransform(transform);
}