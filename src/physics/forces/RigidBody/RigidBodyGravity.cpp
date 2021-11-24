#include "physics/forces/RigidBody/RigidBodyGravity.hpp"

RigidBodyGravity::RigidBodyGravity()
{
	m_gravity.setZ(-9.81f);
}

RigidBodyGravity::RigidBodyGravity(float gravityValue)
{
	m_gravity.setZ(-gravityValue);
}

RigidBodyGravity::~RigidBodyGravity()
{

}

void RigidBodyGravity::setGravity(float value)
{
	m_gravity.setZ(-value);
}

void RigidBodyGravity::updateForce(RigidBody* rigidBody, float duration)
{
	if (rigidBody->getIsResting())
	{
		return;
	}

	if (rigidBody->getInverseMass() == 0)
	{
		// Infinite mass
		return;
	}

	rigidBody->addForce(m_gravity * rigidBody->getMass());
}