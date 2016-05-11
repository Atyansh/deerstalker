#include "Player.h"

#include "BulletCollision/CollisionShapes/btMultiSphereShape.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"
#include "BulletCollision/CollisionDispatch/btCollisionWorld.h"
#include "LinearMath/btDefaultMotionState.h"
#include "Player.h"

Player::Player(int id, btScalar height, btScalar width, btScalar stepHeight,btCollisionShape * m_shape) {
	m_rayLambda[0] = 1.0;
	m_rayLambda[1] = 1.0;
	m_halfHeight = 1.0;
	m_turnAngle = 0.0;
	m_maxLinearVelocity = 10.0;
	m_walkVelocity = 8.0; // meters/sec
	m_turnVelocity = 1.0; // radians/sec
	m_rigidBody = NULL;

	

	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(btVector3(0.0, 2.0, 0.0));
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo cInfo(1.0, myMotionState, m_shape);
	m_rigidBody = new btRigidBody(cInfo);
	// kinematic vs. static doesn't work
	//m_rigidBody->setCollisionFlags( m_rigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	m_rigidBody->setSleepingThresholds(0.0, 0.0);
	m_rigidBody->setAngularFactor(0.0);
}

Player::~Player() {
	if (m_shape) {
		delete m_shape;
	}

	if (m_rigidBody) {
		delete m_rigidBody;
		m_rigidBody = 0;
	}
}

btCollisionObject* Player::getCollisionObject() {
	return m_rigidBody;
}

void Player::preStep(const btCollisionWorld* collisionWorld) {
	btTransform xform;
	m_rigidBody->getMotionState()->getWorldTransform(xform);
	btVector3 down = -xform.getBasis()[1];
	btVector3 forward = xform.getBasis()[2];
	down.normalize();
	forward.normalize();

	m_raySource[0] = xform.getOrigin();
	m_raySource[1] = xform.getOrigin();

	m_rayTarget[0] = m_raySource[0] + down * m_halfHeight * btScalar(1.1);
	m_rayTarget[1] = m_raySource[1] + forward * m_halfHeight * btScalar(1.1);

	class ClosestNotMe : public btCollisionWorld::ClosestRayResultCallback {
		public:
			ClosestNotMe(btRigidBody* me) 
				: btCollisionWorld::ClosestRayResultCallback(btVector3(0.0, 0.0, 0.0), btVector3(0.0, 0.0, 0.0)) {
				m_me = me;
			}

			btScalar addSingleResult(btCollisionWorld::LocalRayResult& rayResult, bool normalInWorldSpace) {
				if (rayResult.m_collisionObject == m_me) {
					return 1.0;
				}

				return ClosestRayResultCallback::addSingleResult(rayResult, normalInWorldSpace);
			}
		protected:
			btRigidBody* m_me;
	};

	ClosestNotMe rayCallback(m_rigidBody);

	int i = 0;
	for (i = 0; i < 2; i++) {
		rayCallback.m_closestHitFraction = 1.0;
		collisionWorld->rayTest(m_raySource[i], m_rayTarget[i], rayCallback);
		if (rayCallback.hasHit()) {
			m_rayLambda[i] = rayCallback.m_closestHitFraction;
		}
		else {
			m_rayLambda[i] = 1.0;
		}
	}
}

void Player::playerStep(const btCollisionWorld* dynaWorld, btScalar dt,
	protos::Event_Direction dir){
	btTransform xform;
	m_rigidBody->getMotionState()->getWorldTransform(xform);

	/* Handle turning */
	if (dir == protos::Event_Direction_LEFT) {
		m_turnAngle -= dt * m_turnVelocity;
	}
	if (dir == protos::Event_Direction_RIGHT) {
		m_turnAngle += dt * m_turnVelocity;
	}

	xform.setRotation(btQuaternion(btVector3(0.0, 1.0, 0.0), m_turnAngle));

	btVector3 linearVelocity = m_rigidBody->getLinearVelocity();
	btScalar speed = m_rigidBody->getLinearVelocity().length();

	btVector3 forwardDir = xform.getBasis()[2];
	forwardDir.normalize();
	btVector3 walkDirection = btVector3(0.0, 0.0, 0.0);
	btScalar walkSpeed = m_walkVelocity * dt;

	if (dir == protos::Event_Direction_FORWARD) {
		walkDirection += forwardDir;
	}
	if (dir == protos::Event_Direction_BACKWARD) {
		walkDirection -= forwardDir;
	}


	if ( dir!= protos::Event_Direction_FORWARD && dir != protos::Event_Direction_BACKWARD && onGround()) {
		/* Dampen when on the ground and not being moved by the player */
		linearVelocity *= btScalar(0.2);
		m_rigidBody->setLinearVelocity(linearVelocity);
	}
	else {
		if (speed < m_maxLinearVelocity) {
			btVector3 velocity = linearVelocity + walkDirection * walkSpeed;
			m_rigidBody->setLinearVelocity(velocity);
		}
	}

	m_rigidBody->getMotionState()->setWorldTransform(xform);
	m_rigidBody->setCenterOfMassTransform(xform);
}

bool Player::canJump() const {
	return onGround();
}

void Player::jump() {
	if (!canJump()) {
		return;
	}

	btTransform xform;
	m_rigidBody->getMotionState()->getWorldTransform(xform);
	btVector3 up = xform.getBasis()[1];
	up.normalize();
	btScalar magnitude = (btScalar(1.0) / m_rigidBody->getInvMass()) * btScalar(8.0);
	m_rigidBody->applyCentralImpulse(up * magnitude);
}

bool Player::onGround() const {
	return m_rayLambda[0] < btScalar(1.0);
}

void Player::reset() {
}

void Player::warp(const btVector3& origin) {
}

void Player::registerPairCacheAndDispatcher(btOverlappingPairCache* pairCache, btCollisionDispatcher* dispatcher) {

}

int Player::getId() {
	return id_;
}
btMotionState * Player::getMotionState() {
	return m_rigidBody->getMotionState();
}
