#ifndef FRAYIEN_PARTICLEFORCEREGISTRY
#define FRAYIEN_PARTICLEFORCEREGISTRY

#include "physics/forces/ParticleForceGenerator.hpp"

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

private:
	Registry m_registry;

};

#endif // FRAYIEN_PARTICLEFORCEREGISTRY
