#ifndef MPJVP_PHYSICSENGINE
#define MPJVP_PHYSICSENGINE

#include <unordered_map>

#include "GameState.hpp"
#include "physics/forces/Particle/ParticleForceRegistry.hpp"
#include "physics/forces/RigidBody/RigidBodyForceRegistry.hpp"
#include "physics/contacts/Particle/ParticleContactResolver.hpp"
#include "physics/contacts/Particle/ParticleContactGenerator.hpp"
#include "collision/octree.hpp"
#include "physics/contacts/RigidBody/Sphere.hpp"
#include "collision/CollisionDetector.hpp"

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

	inline void clear()
    {
        m_particleForceRegistry.clear();
        m_rigidBodyForceRegistry.clear();
    }

private :
	ParticleForceRegistry m_particleForceRegistry;
	RigidBodyForceRegistry m_rigidBodyForceRegistry;
	ParticleContactResolver m_particleContactResolver;
	CollisionDetector m_collisionDetector;
};

#endif // MPJVP_PHYSICSENGINE
