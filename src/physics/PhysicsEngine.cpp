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

    // Broad phase
    m_octree.clear();
    for (auto & boundingVolume : gameState.getBoundingVolumeSphere())
    {
        m_octree.insert(boundingVolume.get());
    }

    auto possibleCollisions = m_octree.findAllPossibleCollisions();

    // 3 : Narrow phase


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

void PhysicsEngine::registerForce(RigidBody* rigidBody, RigidBodyForceGenerator* rigidBodyForceGenerator, float duration)
{
    m_rigidBodyForceRegistry.addForce(rigidBody, rigidBodyForceGenerator, duration);
}