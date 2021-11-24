#include "physics/contacts/Particle/NaiveParticleContactGenerator.hpp"

NaiveParticleContactGenerator::NaiveParticleContactGenerator(std::vector<Particle*> particles) :
	m_particles{ particles }
{
}

float NaiveParticleContactGenerator::getCurrentDistance(Particle* particleOne, Particle* particleTwo) const
{
	Vector3f particleDistance{ particleOne->getPosition() - particleTwo->getPosition() };

	return particleDistance.norm();
}

void NaiveParticleContactGenerator::addContact(std::vector<ParticleContact>& contacts) const
{
	for (int i = 0; i < m_particles.size()-1; i++) {

		for (int j = i+1; j < m_particles.size(); j++) {

			if (getCurrentDistance(m_particles[i], m_particles[j]) <= m_particles[i]->getRadius() + m_particles[j]->getRadius()) {
				Vector3f vectorToNormalize{ m_particles[i]->getPosition() - m_particles[j]->getPosition() };

				contacts.emplace_back(m_particles[i], m_particles[j], std::abs(m_particles[i]->getRadius() - (getCurrentDistance(m_particles[i], m_particles[j]) - m_particles[j]->getRadius())), vectorToNormalize.normalize());
			}
		}
	}
}