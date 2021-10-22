#include "physics/contacts/ParticleRod.hpp"

ParticleRod::ParticleRod(Particle* particleOne, Particle* particleTwo, float length) :
	ParticleLink{particleOne, particleTwo},
	m_lengthConstraint{ length }
{
}

void ParticleRod::addContact(std::vector<ParticleContact>& contacts) const {

	if (getCurrentLength() > m_lengthConstraint) {
		Vector3f vectorToNormalize{ m_particles[1]->getPosition() - m_particles[0]->getPosition() };

		contacts.emplace_back(m_particles[0], m_particles[1], std::abs(getCurrentLength() - m_lengthConstraint), vectorToNormalize.normalize());
	}
	else if (getCurrentLength() < m_lengthConstraint) {
		Vector3f vectorToNormalize{ m_particles[0]->getPosition() - m_particles[1]->getPosition() };

		contacts.emplace_back(m_particles[0], m_particles[1], std::abs(getCurrentLength() - m_lengthConstraint), vectorToNormalize.normalize());
	}

	
}