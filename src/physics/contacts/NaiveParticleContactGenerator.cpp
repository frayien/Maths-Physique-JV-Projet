#include "physics/contacts/NaiveParticleContactGenerator.hpp"

NaiveParticleContactGenerator::NaiveParticleContactGenerator(Particle* particleOne, Particle* particleTwo) :
	ParticleLink{ particleOne, particleTwo }
{
}

void NaiveParticleContactGenerator::addContact(std::vector<ParticleContact>& contacts) const
{
	if (getCurrentDistance() <= m_particles[0]->getRadius() + m_particles[1]->getRadius()) {
		Vector3f vectorToNormalize{ m_particles[1]->getPosition() - m_particles[0]->getPosition() };

		contacts.emplace_back(m_particles[0], m_particles[1], std::abs(m_particles[0]->getRadius() - (getCurrentDistance() - m_particles[1]->getRadius())), vectorToNormalize.normalize());
	}
}