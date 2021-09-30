#ifndef MPJVP_PARTICLEFORCEREGISTRY
#define MPJVP_PARTICLEFORCEREGISTRY

#include "physics/ParticleForceGenerator.hpp"

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

#endif // MPJVP_PARTICLEFORCEREGISTRY
