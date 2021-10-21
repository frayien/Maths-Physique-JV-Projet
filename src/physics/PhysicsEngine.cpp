#include "physics/PhysicsEngine.hpp"

#include <vector>

PhysicsEngine::PhysicsEngine()
{

}

PhysicsEngine::~PhysicsEngine()
{

}

void PhysicsEngine::update(float deltaTime, GameState & gameState)
{
    m_particleForceRegistry.update(deltaTime);

    for (auto & particleItem : gameState.getParticles())
    {
        particleItem.second->integrate(deltaTime);
    }
}

void PhysicsEngine::registerForce(Particle * particle, ParticleForceGenerator * particleForceGenerator, float duration)
{
    m_particleForceRegistry.addForce(particle, particleForceGenerator, duration);
}