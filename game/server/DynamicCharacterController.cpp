#include "BulletCollision/CollisionShapes/btMultiSphereShape.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"
#include "BulletCollision/CollisionDispatch/btCollisionWorld.h"
#include "LinearMath/btDefaultMotionState.h"
#include "DynamicCharacterController.h"

DynamicCharacterController::DynamicCharacterController(btCollisionObject* body)
{
	m_rayLambda[0] = 1.0;
	m_rayLambda[1] = 1.0;
	m_halfHeight = 1.0;
	m_turnAngle = 0.0;
	m_maxLinearVelocity = 10.0;
	m_walkVelocity = 8.0; // meters/sec
	m_turnVelocity = 1.0; // radians/sec
	m_shape = body->getCollisionShape();
	m_rigidBody = NULL;
	setup(100.0, 1.0, 20.0);
}

DynamicCharacterController::~DynamicCharacterController()
{
}

void DynamicCharacterController::setup(btScalar height, btScalar width, btScalar stepHeight)
{
	/*
	btVector3 spherePositions[2];
	btScalar sphereRadii[2];

	sphereRadii[0] = width;
	sphereRadii[1] = width;
	spherePositions[0] = btVector3(0.0, (height / btScalar(2.0) - width), 0.0);
	spherePositions[1] = btVector3(0.0, (-height / btScalar(2.0) + width), 0.0);

	m_halfHeight = height / btScalar(2.0);
	*/
	//m_shape = new btMultiSphereShape(&spherePositions[0], &sphereRadii[0], 2);

	m_halfHeight = 5.0;

	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(btVector3(0.0, 10.0, 0.0));
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo cInfo(1.0, myMotionState, m_shape);
	m_rigidBody = new btRigidBody(cInfo);
	// kinematic vs. static doesn't work
	//m_rigidBody->setCollisionFlags( m_rigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
	m_rigidBody->setSleepingThresholds(0.0, 0.0);
	m_rigidBody->setAngularFactor(0.0);

}

void DynamicCharacterController::destroy()
{
	if (m_shape)
	{
		delete m_shape;
	}

	if (m_rigidBody)
	{
		delete m_rigidBody;
		m_rigidBody = 0;
	}
}

btCollisionObject* DynamicCharacterController::getCollisionObject()
{
	return m_rigidBody;
}

void DynamicCharacterController::preStep(btCollisionWorld* collisionWorld)
{
	btTransform xform;
	m_rigidBody->getMotionState()->getWorldTransform(xform);
	btVector3 down = -xform.getBasis()[1];
	btVector3 forward = xform.getBasis()[2];
	down.normalize();
	forward.normalize();

	m_raySource[0] = xform.getOrigin() + btVector3(0.0, 0.1, 0.0);
	m_raySource[1] = xform.getOrigin();

	m_rayTarget[0] = m_raySource[0] + down * btScalar(1.1);
	m_rayTarget[1] = m_raySource[1] + forward * m_halfHeight * btScalar(1.1);

	std::cerr << "sourceY: " << m_raySource[0].getY() << std::endl;
	std::cerr << "targetY: " << m_rayTarget[0].getY() << std::endl;

	class ClosestNotMe : public btCollisionWorld::ClosestRayResultCallback
	{
	public:
		ClosestNotMe(btRigidBody* me) : btCollisionWorld::ClosestRayResultCallback(btVector3(0.0, 0.0, 0.0), btVector3(0.0, 0.0, 0.0))
		{
			m_me = me;
		}

		virtual btScalar addSingleResult(btCollisionWorld::LocalRayResult& rayResult, bool normalInWorldSpace)
		{
			if (rayResult.m_collisionObject == m_me)
				return 1.0;

			return ClosestRayResultCallback::addSingleResult(rayResult, normalInWorldSpace
			);
		}
	protected:
		btRigidBody* m_me;
	};

	ClosestNotMe rayCallback(m_rigidBody);

	int i = 0;
	for (i = 0; i < 2; i++)
	{
		rayCallback.m_closestHitFraction = 1.0;
		collisionWorld->rayTest(m_raySource[i], m_rayTarget[i], rayCallback);
		if (rayCallback.hasHit())
		{
			std::cerr << "hasHit" << std::endl;
			m_rayLambda[i] = rayCallback.m_closestHitFraction;
		}
		else {
			std::cerr << "NOT HIT" << std::endl;
			m_rayLambda[i] = 1.0;
		}
	}

	btVector3 linearVelocity = m_rigidBody->getLinearVelocity();
	linearVelocity *= btVector3(0.2, 1, 0.2);
	m_rigidBody->setLinearVelocity(linearVelocity);
}

void DynamicCharacterController::playerStep(const btCollisionWorld*, btVector3& dir)
{
	btTransform xform;
	m_rigidBody->getMotionState()->getWorldTransform(xform);

	/* Handle turning */
	/*
	if (left)
		m_turnAngle -= dt * m_turnVelocity;
	if (right)
		m_turnAngle += dt * m_turnVelocity;
	*/

	//xform.setRotation(btQuaternion(btVector3(0.0, 1.0, 0.0), m_turnAngle));

	btVector3 linearVelocity = m_rigidBody->getLinearVelocity();
	//btScalar speed = m_rigidBody->getLinearVelocity().length();

	//btVector3 forwardDir = xform.getBasis()[2];
	//forwardDir.normalize();
	//btVector3 walkDirection = btVector3(0.0, 0.0, 0.0);
	btScalar walkSpeed = 30;

	/*
	if (forward) {
		walkDirection += forwardDir;
	}
	if (backward) {
		walkDirection -= forwardDir;
	}
	*/

	setLookDirection(m_rigidBody, dir);

	btVector3 velocity = dir * walkSpeed;
	velocity.setY(m_rigidBody->getLinearVelocity().getY());
	m_rigidBody->setLinearVelocity(velocity);

	m_rigidBody->getMotionState()->setWorldTransform(xform);

	/*
	if (right || left) {
		m_rigidBody->setCenterOfMassTransform(xform);
	}
	*/
}

bool DynamicCharacterController::canJump() const
{
	return onGround();
}

void DynamicCharacterController::jump()
{
	if (!canJump())
		return;

	btTransform xform;
	m_rigidBody->getMotionState()->getWorldTransform(xform);
	btVector3 up = xform.getBasis()[1];
	up.normalize();
	btScalar magnitude = 15;
	m_rigidBody->applyCentralImpulse(up * magnitude);
}

bool DynamicCharacterController::onGround() const
{
	return m_rayLambda[0] < btScalar(1.0);
}

void DynamicCharacterController::warp(const btVector3& origin)
{
}
void DynamicCharacterController::registerPairCacheAndDispatcher(btOverlappingPairCache* pairCache, btCollisionDispatcher* dispatcher)
{

}

void DynamicCharacterController::setLookDirection(btRigidBody* body, const btVector3& newLook) {
	// assume that "forward" for the player in local-frame is +zAxis
	// and that the player is constrained to rotate about yAxis (+yAxis is "up")
	btVector3 localLook(0.0f, 0.0f, 1.0f); // zAxis
	btVector3 rotationAxis(0.0f, 1.0f, 0.0f); // yAxis

	// compute currentLook and angle
	btTransform transform = body->getCenterOfMassTransform();
	btQuaternion rotation = transform.getRotation();
	btVector3 currentLook = quatRotate(rotation, localLook); //rotation * localLook;
	btScalar angle = currentLook.angle(newLook);

	// compute new rotation
	btQuaternion deltaRotation(rotationAxis, angle);
	btQuaternion newRotation = deltaRotation * rotation;

	// apply new rotation
	transform.setRotation(newRotation);
	body->setCenterOfMassTransform(transform);
}