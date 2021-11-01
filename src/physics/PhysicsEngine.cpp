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
    // Update forces
    m_particleForceRegistry.update(deltaTime);

    // Update particles
    for (auto & [label, particle] : gameState.getParticles())
    {
        particle->integrate(deltaTime);
    }

    // Generate contacts
    std::vector<ParticleContact> contacts;
    for(auto & [label, contactGenerator] : gameState.getParticleContactGenerators())
    {
        contactGenerator->addContact(contacts);
    }

    // Resolve contacts
    m_particleContactResolver.resolveContacts(contacts, deltaTime);
}

void PhysicsEngine::registerForce(Particle * particle, ParticleForceGenerator * particleForceGenerator, float duration)
{
    m_particleForceRegistry.addForce(particle, particleForceGenerator, duration);
}