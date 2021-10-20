#ifndef MPJVP_PHYSICSENGINE
#define MPJVP_PHYSICSENGINE

#include "GameState.hpp"
#include "ParticleForceRegistry.hpp"
#include "Particle.hpp"
#include "ParticleForceGenerator.hpp"

class PhysicsEngine
{

public:
	PhysicsEngine();
	~PhysicsEngine();

	void update(float deltaTime, GameState & gameState);
	void registerForce(Particle * particle, ParticleForceGenerator * particleForceGenerator, float duration);

    inline ParticleForceRegistry & getParticleForceRegistry() { return m_particleForceRegistry; }

private:
	ParticleForceRegistry m_particleForceRegistry;
};

#endif // MPJVP_PHYSICSENGINE
