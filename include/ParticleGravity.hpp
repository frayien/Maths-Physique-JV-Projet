#ifndef MPJVP_PARTICLEGRAVITY
#define MPJVP_PARTICLEGRAVITY

#include "ParticleForceGenerator.hpp"
#include "Vector3f.hpp"

class ParticleGravity : public ParticleForceGenerator 
{
private:
	Vector3f m_gravity;
public:
	ParticleGravity();
	ParticleGravity(float gravityValue);
	~ParticleGravity();
	void UpdateForce(Particle* particle, float duration);
	
};

#endif // MPJVP_PARTICLEGRAVITY