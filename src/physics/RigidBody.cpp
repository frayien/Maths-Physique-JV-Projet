#include "physics/RigidBody.hpp"

RigidBody::RigidBody()
{

}

RigidBody::RigidBody(Vector3f position, float mass, float damping) :
	m_position {position},
	m_damping {damping}
{
	m_inverseMass = 1.0f / mass;
}

RigidBody::~RigidBody()
{

}

void RigidBody::integrate(float deltaTime)
{
	if (m_inverseMass == 0.0f)
	{
		// Don't integrate particle with infinite mass
		return;
	}

	Vector3f acceleration = m_inverseMass * m_totalForce;

	m_position += m_velocity * deltaTime;

	// change orientation (quaternion) here
	m_orientation = m_orientation.update(m_rotation, deltaTime);

	m_velocity += acceleration * deltaTime;

	// Rotation update
	m_rotation += Vector{ 0,0,0 };

	m_totalForce = { 0.0f, 0.0f, 0.0f };
}

std::ostream& operator<<(std::ostream& out, const RigidBody& a)
{
	return out << "position : " << a.m_position << std::endl
		<< "velocity : " << a.m_velocity << std::endl
		<< "totalForce : " << a.m_totalForce << std::endl
		<< "inverseMass : " << a.m_inverseMass << std::endl
		<< "damping : " << a.m_damping << std::endl << std::endl;
}

void RigidBody::calculateDerivedData()
{
	m_orientation.normalize();

	// Calculate transformMatrix here
	m_transformMatrix.setOrientationAndPosition(m_orientation, m_position);
}