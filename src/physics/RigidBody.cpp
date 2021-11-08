#include "physics/RigidBody.hpp"

RigidBody::RigidBody()
{

}

RigidBody::RigidBody(Vector3f position, float mass, float damping, float angularDamping, bool isResting) :
	m_position {position},
	m_isResting(isResting),
	m_damping {damping},
	m_angularDamping {angularDamping}
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

	// linear acceleration
	Vector3f acceleration = m_inverseMass * m_totalForce;

	// angular acceleration
	Vector3f angularAcceleration = m_inverseInertiaMoment * m_totalTorque;

	// linear velocity
	m_velocity = m_velocity * m_damping + acceleration * deltaTime;

	// angular velocity
	m_angularVelocity = m_angularVelocity * m_angularDamping + angularAcceleration * deltaTime;

	// position
	m_position += m_velocity * deltaTime;

	// orientation
	m_orientation = m_orientation.update(m_angularVelocity, deltaTime);

	// derived values' calcul
	calculateDerivedData();

	// put to 0 totalForce and totalTorque
	clearAccumulator();
}

void RigidBody::addForce(const Vector3f& force)
{
	m_totalForce += force;
	m_isResting = false;
}


void RigidBody::addForceAtPoint(const Vector3f& force, const Vector3f& worldPoint)
{
	// Force
	m_totalForce += force;

	// Torque
	// Convert to coordinates relative to center of mass
	Vector3f point = worldPoint - m_position;
	m_totalTorque += point.crossProduct(force);

	m_isResting = false;
}

void RigidBody::addForceAtBodyPoint(const Vector3f& force, const Vector3f& localPoint)
{
	// Force
	m_totalForce += force;

	// Torque
	// Convert to coordinates relative to the world
	Vector3f worldPoint = m_transformMatrix * localPoint;
	addForceAtPoint(force, worldPoint);

	m_isResting = false;
}

void RigidBody::clearAccumulator()
{
	m_totalForce = { 0.0f, 0.0f, 0.0f };
	m_totalTorque = { 0.0f, 0.0f, 0.0f };
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
	m_orientation = m_orientation.normalize();

	// Calculate transformMatrix here
	m_transformMatrix.setOrientationAndPosition(m_orientation, m_position);

	// -----------Calculate inverse of inertia moment here------------------
	

}