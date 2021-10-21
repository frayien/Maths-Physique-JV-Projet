#include "physics/ParticleRod.hpp"

ParticleRod::ParticleRod(float length) : 
	m_lengthConstraint{ length }
{
}

void ParticleRod::addContact(std::vector<ParticleContact>& contacts) const {

	if (getCurrentLength() > m_lengthConstraint) {
		Vector3f vectorToNormalize{ m_particles[0]->getPosition() - m_particles[1]->getPosition() };

		contacts.emplace_back(m_particles[0], m_particles[1], std::abs(getCurrentLength() - m_lengthConstraint), vectorToNormalize.normalize());
	}
	else if (getCurrentLength() < m_lengthConstraint) {
		Vector3f vectorToNormalize{ m_particles[1]->getPosition() - m_particles[0]->getPosition() };

		contacts.emplace_back(m_particles[0], m_particles[1], std::abs(getCurrentLength() - m_lengthConstraint), vectorToNormalize.normalize());
	}

	
}