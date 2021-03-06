#include "physics/contacts/Particle/ParticleRod.hpp"

ParticleRod::ParticleRod(Particle* particleOne, Particle* particleTwo, float length) :
	ParticleLink{particleOne, particleTwo},
	m_lengthConstraint{ length }
{
}

void ParticleRod::addContact(std::vector<ParticleContact>& contacts) const {

	if (getCurrentDistance() > m_lengthConstraint) {
		Vector3f vectorToNormalize{ m_particles[1]->getPosition() - m_particles[0]->getPosition() };

		contacts.emplace_back(m_particles[0], m_particles[1], std::abs(getCurrentDistance() - m_lengthConstraint), vectorToNormalize.normalize());
	}
	else if (getCurrentDistance() < m_lengthConstraint) {
		Vector3f vectorToNormalize{ m_particles[0]->getPosition() - m_particles[1]->getPosition() };

		contacts.emplace_back(m_particles[0], m_particles[1], std::abs(getCurrentDistance() - m_lengthConstraint), vectorToNormalize.normalize());
	}

	
}