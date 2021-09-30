#include "physics/ParticleGravity.hpp"

ParticleGravity::ParticleGravity()
{
	m_gravity.setY(-9.81);
}

ParticleGravity::ParticleGravity(float gravityValue)
{
	m_gravity.setY(-gravityValue);
}

ParticleGravity::~ParticleGravity()
{

}

void ParticleGravity::UpdateForce(Particle* particle, float duration)
{
	particle->setTotalForce(particle->getTotalForce() + m_gravity);
}