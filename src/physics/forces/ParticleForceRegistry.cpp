#include "physics/forces/ParticleForceRegistry.hpp"

ParticleForceRegistry::ParticleForceRegistry() 
{
}

ParticleForceRegistry::~ParticleForceRegistry() 
{
}

void ParticleForceRegistry::addForce(Particle* particle, ParticleForceGenerator* particleForceGenerator, float duration)
{
	ParticleForceEntry newEntry;

	newEntry.particle = particle;
	newEntry.particleForceGenerator = particleForceGenerator;

	m_registry.push_back(newEntry);
}

void ParticleForceRegistry::removeForce(ParticleForceGenerator* _particleForceGenerator)
{
	for (std::vector<ParticleForceEntry>::iterator it = m_registry.begin(); it != m_registry.end(); ++it) {
		if (it->particleForceGenerator == _particleForceGenerator) {
			m_registry.erase(it);
		}
	}
}

void ParticleForceRegistry::update(float deltaTime)
{
	for (auto & [particle, particleForceGenerator] : m_registry)
	{
		particleForceGenerator->updateForce(particle, deltaTime);
	}
}