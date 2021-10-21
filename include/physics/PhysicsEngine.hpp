#ifndef MPJVP_PHYSICSENGINE
#define MPJVP_PHYSICSENGINE

#include "physics/ParticleForceRegistry.hpp"

class PhysicsEngine
{

public:
	PhysicsEngine();
	~PhysicsEngine();

	void update(float deltaTime, std::vector<Particle *> & particles);

    inline ParticleForceRegistry & getParticleRegistry() { return m_particleRegistry; }

private :
	ParticleForceRegistry m_particleRegistry;
};

#endif // MPJVP_PHYSICSENGINE
