#ifndef FRAYIEN_PARTICLEFORCEREGISTRY
#define FRAYIEN_PARTICLEFORCEREGISTRY

#include "physics/forces/Particle/ParticleForceGenerator.hpp"

class ParticleForceRegistry 
{
public:
	struct ParticleForceEntry
	{
		Particle* particle;
		ParticleForceGenerator* particleForceGenerator;
	};

	using Registry = std::vector<ParticleForceEntry>;

	ParticleForceRegistry();
	virtual ~ParticleForceRegistry();

	void addForce(Particle* particle, ParticleForceGenerator* particleForceGenerator, float duration);
	void removeForce(ParticleForceGenerator* _particleForceGenerator);

	void update(float deltaTime);

	inline void clear()
	{
		m_registry.clear();
	}

private:
	Registry m_registry;

};

#endif // FRAYIEN_PARTICLEFORCEREGISTRY
