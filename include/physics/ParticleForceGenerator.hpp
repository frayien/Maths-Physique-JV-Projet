#ifndef MPJVP_PARTICLEFORCEGENERATOR
#define MPJVP_PARTICLEFORCEGENERATOR

#include "physics/Particle.hpp"
#include "physics/ParticleContact.hpp"

class ParticleForceGenerator
{
public:
	ParticleForceGenerator() = default;
	virtual ~ParticleForceGenerator() = default;

	virtual void updateForce(Particle* particle, float duration) = 0;
};

#endif // MPJVP_PARTICLEFORCEGENERATOR