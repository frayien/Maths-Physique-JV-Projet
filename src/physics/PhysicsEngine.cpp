#include "physics/PhysicsEngine.hpp"

#include <vector>

PhysicsEngine::PhysicsEngine()
{
    numberOfSphereAndSphereContact = 0;
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

    std::unordered_map<BoundingVolumeSphere*, std::vector<Primitive*>> linksBetweenBoundingVolumesAndPrimitives = gameState.getLinksBetweenBoundingVolumesAndPrimitives();

    // 3 : Narrow phase
    for (auto & boundingVolumePair : possibleCollisions) {

        std::unordered_map<BoundingVolumeSphere*, std::vector<Primitive*>>::iterator firstPrimitiveVectorIterator = linksBetweenBoundingVolumesAndPrimitives.find(boundingVolumePair.first);
        std::unordered_map<BoundingVolumeSphere*, std::vector<Primitive*>>::iterator secondPrimitiveVectorIterator = linksBetweenBoundingVolumesAndPrimitives.find(boundingVolumePair.second);

        if (firstPrimitiveVectorIterator != linksBetweenBoundingVolumesAndPrimitives.end() || secondPrimitiveVectorIterator != linksBetweenBoundingVolumesAndPrimitives.end()) continue;

        std::vector<Primitive*> firstPrimitiveVector = firstPrimitiveVectorIterator->second;
        std::vector<Primitive*> secondPrimitiveVector = secondPrimitiveVectorIterator->second;

        for (Primitive* primitive1 : firstPrimitiveVector) {
            for (Primitive* primitive2 : secondPrimitiveVector) {

                // If the first primitive is a sphere
                if (dynamic_cast<Sphere*>(primitive1) != nullptr) {

                    // If the second primitive is a sphere
                    if (dynamic_cast<Sphere*>(primitive2) != nullptr) {
                        auto sphereAndSphereContact = std::make_unique<SphereAndSphereContact>(dynamic_cast<Sphere*>(primitive1), dynamic_cast<Sphere*>(primitive2));
                        gameState.addRigidBodyContactGenerator("sphereAndContactContact_" + numberOfSphereAndSphereContact, std::move(sphereAndSphereContact));
                        numberOfSphereAndSphereContact++;
                    }

                }

            }
        }
    }

    // Generate RigidBody contacts
    std::vector<RigidBodyContact> rigidBodyContacts;
    for (auto& [label, contactGenerator] : gameState.getRigidBodyContactGenerators())
    {
        contactGenerator->addContact(rigidBodyContacts);

        // We add the contact, then proceed to erase the element
        gameState.getRigidBodyContactGenerators().erase(label);
    }

    // We put to 0 all the numberOfContact variables
    numberOfSphereAndSphereContact = 0;
    // other variables put to 0 here...

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