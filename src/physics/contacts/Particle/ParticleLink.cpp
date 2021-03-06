#include "physics/contacts/Particle/ParticleLink.hpp"

ParticleLink::ParticleLink(Particle* particleOne, Particle* particleTwo){
	m_particles[0] = particleOne;
	m_particles[1] = particleTwo;
}

float ParticleLink::getCurrentDistance() const {
	Vector3f particleDistance{ m_particles[0]->getPosition() - m_particles[1]->getPosition()};

	return particleDistance.norm();
}