#include "physics/Particle.hpp"

Particle::Particle()
{
	m_inverseMass = 1.0f;
	m_damping = 0.999f;
}

Particle::Particle(Vector3f position, float mass, float damping, bool isResting, float radius) :
	m_damping(damping),
	m_position(position),
	m_isResting(isResting),
	m_radius(radius)
{
	m_inverseMass = 1.0f / mass;
}

Particle::~Particle()
{
}

void Particle::integrate(float deltaTime)
{
	if (m_inverseMass == 0.0f)
	{
		// Don't integrate particle with infinite mass
		return;
	}

	Vector3f acceleration = m_inverseMass * m_totalForce;

	m_position += m_velocity * deltaTime + (deltaTime * deltaTime * acceleration / 2.0f);
	m_velocity = m_velocity * m_damping + acceleration * deltaTime;
	
	m_totalForce = {0.0f, 0.0f, 0.0f};
}

std::ostream& operator<<(std::ostream& out, const Particle& a)
{
	return out << "position : "     << a.m_position     << std::endl 
	           << "velocity : "     << a.m_velocity     << std::endl 
			   << "totalForce : "   << a.m_totalForce   << std::endl 
			   << "inverseMass : "  << a.m_inverseMass  << std::endl 
			   << "damping : "      << a.m_damping      << std::endl << std::endl;
}