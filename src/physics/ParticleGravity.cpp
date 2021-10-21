#include "physics/ParticleGravity.hpp"

ParticleGravity::ParticleGravity()
{
	m_gravity.setZ(-9.81f);
}

ParticleGravity::ParticleGravity(float gravityValue)
{
	m_gravity.setZ(-gravityValue);
}

ParticleGravity::~ParticleGravity()
{

}

void ParticleGravity::setGravity(float value)
{
	m_gravity.setZ(-value);
}

void ParticleGravity::updateForce(Particle* particle, float duration)
{
	if (particle->getInverseMass() == 0)
	{
		// Infinite mass
		return;
	}

	particle->addForce(m_gravity * particle->getMass());
}