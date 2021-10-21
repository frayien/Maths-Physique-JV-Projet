#include "physics/PhysicsEngine.hpp"

#include <vector>

PhysicsEngine::PhysicsEngine()
{

}

PhysicsEngine::~PhysicsEngine()
{

}

void PhysicsEngine::update(float deltaTime, std::vector<Particle *> & particles, std::vector<ParticleContactGenerator *> & contactGenerators)
{
    // Generate contacts
    std::vector<ParticleContact> contacts;
    for(ParticleContactGenerator * contactGenerator : contactGenerators)
    {
        contactGenerator->addContact(contacts);
    }

    // Resolve contacts
    m_particleContactResolver.resolveContacts(contacts, deltaTime);

    // Update forces
    m_particleRegistry.update(deltaTime);

    // Update particles
    for (Particle * p : particles)
    {
        p->integrate(deltaTime);
    }
}