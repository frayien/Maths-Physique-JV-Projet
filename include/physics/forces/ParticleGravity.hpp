#ifndef MPJVP_PARTICLEGRAVITY
#define MPJVP_PARTICLEGRAVITY

#include "physics/forces/ParticleForceGenerator.hpp"
#include "physics/Vector3f.hpp"

class ParticleGravity : public ParticleForceGenerator 
{
private:
	Vector3f m_gravity;
public:
	ParticleGravity();
	ParticleGravity(float gravityValue);
	virtual ~ParticleGravity();

	void setGravity(float value);
	
	void updateForce(Particle* particle, float duration);
	
};

#endif // MPJVP_PARTICLEGRAVITY