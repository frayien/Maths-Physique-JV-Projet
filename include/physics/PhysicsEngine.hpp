#ifndef MPJVP_PHYSICSENGINE
#define MPJVP_PHYSICSENGINE

#include "physics/ParticleForceRegistry.hpp"
#include "physics/ParticleContactResolver.hpp"
#include "physics/ParticleContactGenerator.hpp"

class PhysicsEngine
{

public:
	PhysicsEngine();
	~PhysicsEngine();

	void update(float deltaTime, std::vector<Particle *> & particles, std::vector<ParticleContactGenerator *> & contactGenerators);

    inline ParticleForceRegistry & getParticleRegistry() { return m_particleRegistry; }

private :
	ParticleForceRegistry m_particleRegistry;
	ParticleContactResolver m_particleContactResolver;
};

#endif // MPJVP_PHYSICSENGINE
