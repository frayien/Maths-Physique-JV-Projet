#include "physics/ParticleCable.hpp"

ParticleCable::ParticleCable(float maxLength, float restitution) : 
	m_maxLength {maxLength},
	m_restitution {restitution}
{
}

void ParticleCable::addContact(std::vector<ParticleContact>& contacts) const {

	if (getCurrentLength() > m_maxLength) {
		Vector3f vectorToNormalize{ m_particles[0]->getPosition() - m_particles[1]->getPosition() };

		contacts.emplace_back(m_particles[0], m_particles[1], std::abs(getCurrentLength() - m_maxLength), vectorToNormalize.normalize(), m_restitution);
	}

}