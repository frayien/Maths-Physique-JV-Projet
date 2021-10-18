#ifndef MPJVP_PARTICLESPRING
#define MPJVP_PARTICLESPRING

#include "physics/ParticleForceGenerator.hpp"
#include "physics/Particle.hpp"

class ParticleSpring : public ParticleForceGenerator 
{
protected:
	Particle * m_otherParticle;
    float m_k;
    float m_restLength;

public:
	ParticleSpring();
	ParticleSpring(Particle * otherParticle, float k, float restLength);
	virtual ~ParticleSpring();

	void setK(float k);
    void setRestLength(float restLength);
	void setOtherParticle(Particle * other);

	virtual void updateForce(Particle* particle, float duration);
};

#endif // MPJVP_PARTICLESPRING