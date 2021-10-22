#ifndef MPJVP_PARTICLEDRAG
#define MPJVP_PARTICLEDRAG

#include "physics/forces/ParticleForceGenerator.hpp"

class ParticleDrag : public ParticleForceGenerator 
{
private:
	float m_k1;
    float m_k2;

public:
	ParticleDrag(float k1 = 0.0f, float k2 = 0.0f);
	virtual ~ParticleDrag();

	void setK1(float k1);
    void setK2(float k2);

	void updateForce(Particle* particle, float duration);
};

#endif // MPJVP_PARTICLEDRAG