#include "physics/RigidBody.hpp"

RigidBody::RigidBody()
{

}

RigidBody::RigidBody(Vector3f position, float mass, float damping) :
	m_position {position},
	m_damping {damping},
{
	m_inverseMass = 1.0f / mass;
}

RigidBody::~RigidBody()
{

}

void RigidBody::integrate(float duration)
{

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

}