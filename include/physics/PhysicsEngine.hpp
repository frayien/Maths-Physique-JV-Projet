#ifndef MPJVP_PHYSICSENGINE
#define MPJVP_PHYSICSENGINE

#include "GameState.hpp"
#include "physics/forces/ParticleForceRegistry.hpp"
#include "physics/forces/RigidBodyForceRegistry.hpp"
#include "physics/contacts/ParticleContactResolver.hpp"
#include "physics/contacts/ParticleContactGenerator.hpp"

class PhysicsEngine
{

public:
	PhysicsEngine();
	~PhysicsEngine();

	void update(float deltaTime, GameState & gameState);

	void registerForce(Particle * particle, ParticleForceGenerator * particleForceGenerator, float duration);
	void registerForce(RigidBody* rigidBody, RigidBodyForceGenerator* rigidBodyForceGenerator, float duration);

    inline ParticleForceRegistry & getParticleForceRegistry() { return m_particleForceRegistry; }
	inline RigidBodyForceRegistry& getRigidBodyForceRegistry() { return m_rigidBodyForceRegistry; }

private :
	ParticleForceRegistry m_particleForceRegistry;
	RigidBodyForceRegistry m_rigidBodyForceRegistry;
	ParticleContactResolver m_particleContactResolver;
};

#endif // MPJVP_PHYSICSENGINE
