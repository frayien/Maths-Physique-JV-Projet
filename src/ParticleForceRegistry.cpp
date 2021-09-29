#include "ParticleForceRegistry.hpp"

ParticleForceRegistry::ParticleForceRegistry() 
{
}

ParticleForceRegistry::~ParticleForceRegistry() 
{
}

void ParticleForceRegistry::AddForce(Particle* particle, ParticleForceGenerator* particleForceGenerator, float duration)
{
	Entry newEntry;

	newEntry.particle = particle;
	newEntry.particleForceGenerator = particleForceGenerator;

	m_registry.push_back(newEntry);
}

void ParticleForceRegistry::RemoveForce(ParticleForceGenerator* _particleForceGenerator)
{
	for (std::vector<ParticleForceEntry>::iterator it = m_registry.begin(); it != m_registry.end(); ++it) {
		if (it->particleForceGenerator == _particleForceGenerator) {
			m_registry.erase(it);
		}
	}
}