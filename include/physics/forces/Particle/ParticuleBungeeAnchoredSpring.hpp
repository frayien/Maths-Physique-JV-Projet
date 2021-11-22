#ifndef MPJVP_PARTICLEBUNGEEANCHOREDSPRING
#define MPJVP_PARTICLEBUNGEEANCHOREDSPRING

#include "physics/forces/Particle/ParticleAnchoredSpring.hpp"

class ParticleBungeeAnchoredSpring : public ParticleAnchoredSpring 
{
public:
	ParticleBungeeAnchoredSpring();
	ParticleBungeeAnchoredSpring(Vector3f anchor, float k, float restLength);
	virtual ~ParticleBungeeAnchoredSpring();

	void updateForce(Particle* particle, float duration);
};

#endif // MPJVP_PARTICLEBUNGEEANCHOREDSPRING