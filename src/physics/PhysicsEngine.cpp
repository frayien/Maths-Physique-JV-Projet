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
    // Generate contacts
    std::vector<ParticleContact> contacts;
    for(ParticleContactGenerator * contactGenerator : gameState.getContactGenerators())
    {
        contactGenerator->addContact(contacts);
    }

    // Resolve contacts
    m_particleContactResolver.resolveContacts(contacts, deltaTime);

    // Update forces
    m_particleRegistry.update(deltaTime);

    // Update particles
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