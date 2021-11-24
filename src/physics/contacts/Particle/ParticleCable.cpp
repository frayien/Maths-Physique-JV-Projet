#include "physics/contacts/Particle/ParticleCable.hpp"

ParticleCable::ParticleCable(Particle* particleOne, Particle* particleTwo, float maxLength, float restitution) :
	ParticleLink{particleOne, particleTwo},
	m_maxLength {maxLength},
	m_restitution {restitution}
{
}

void ParticleCable::addContact(std::vector<ParticleContact>& contacts) const {

	if (getCurrentDistance() > m_maxLength) {
		Vector3f vectorToNormalize{ m_particles[1]->getPosition() - m_particles[0]->getPosition() };

		contacts.emplace_back(m_particles[0], m_particles[1], std::abs(getCurrentDistance() - m_maxLength), vectorToNormalize.normalize(), m_restitution);
	}

}