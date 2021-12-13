#include "physics/PhysicsEngine.hpp"

#include <vector>

#include <iostream>

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
    m_rigidBodyForceRegistry.update(deltaTime);

    // Update particles
    for (auto & [label, particle] : gameState.getParticles())
    {
        particle->integrate(deltaTime);
    }

    // Update rigid bodies
    for (auto & [label, rigidbody] : gameState.getRigidbodies())
    {
        rigidbody->integrate(deltaTime);
    }

    // Update bounding volumes
    for (auto & boundingVolume : gameState.getBoundingVolumeSphere())
    {
        boundingVolume->updatePosition();
    }

    // Collision detection
    std::vector<RigidBodyContact> allRigidbodyContacts = m_collisionDetector.detectCollisions(gameState);

    // FOR DEMO PHASE 4
    if (allRigidbodyContacts.size() > 0)
    {
        // Display collision data
        for (RigidBodyContact & contact : allRigidbodyContacts)
        {
            std::cout << "Donnees du contact : \n" << contact << std::endl;
        }

        // WHAT TO DO ?
    }

    // --------------------- Resolve RigidBody contacts ------------------------------
    // HERE

    // Generate Particle contacts
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

void PhysicsEngine::registerForce(RigidBody* rigidBody, RigidBodyForceGenerator* rigidBodyForceGenerator, float duration)
{
    m_rigidBodyForceRegistry.addForce(rigidBody, rigidBodyForceGenerator, duration);
}