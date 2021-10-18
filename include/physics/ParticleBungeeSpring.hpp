#ifndef MPJVP_PARTICLEBUNGEESPRING
#define MPJVP_PARTICLEBUNGEESPRING

#include "ParticleSpring.hpp"

class ParticleBungeeSpring : public ParticleSpring 
{
public:
	ParticleBungeeSpring();
	ParticleBungeeSpring(Particle * otherParticle, float k, float restLength);
	virtual ~ParticleBungeeSpring();

	void updateForce(Particle* particle, float duration);
};

#endif // MPJVP_PARTICLEBUNGEESPRING