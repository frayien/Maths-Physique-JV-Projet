#include "collision/CollisionDetector.hpp"
#include "collision/BoundingVolumeSphere.hpp"

std::vector<RigidBodyContact> CollisionDetector::detectCollisions(GameState& gameState)
{
	// ---- Broad phase ----
    m_octree.clear();
    for (auto& boundingVolume : gameState.getBoundingVolumeSphere())
    {
        m_octree.insert(boundingVolume.get());
    }

    auto possibleCollisions = m_octree.findAllPossibleCollisions();

	// ---- Narrow phase ----
    std::unordered_map<BoundingVolumeSphere*, std::vector<Primitive*>> linksBetweenBoundingVolumesAndPrimitives = gameState.getLinksBetweenBoundingVolumesAndPrimitives();
    std::vector<RigidBodyContact> allContacts;

    for (auto& boundingVolumePair : possibleCollisions)
    {
        auto firstPrimitiveVectorIterator = linksBetweenBoundingVolumesAndPrimitives.find(boundingVolumePair.first);
        auto secondPrimitiveVectorIterator = linksBetweenBoundingVolumesAndPrimitives.find(boundingVolumePair.second);

        if (firstPrimitiveVectorIterator != linksBetweenBoundingVolumesAndPrimitives.end() || secondPrimitiveVectorIterator != linksBetweenBoundingVolumesAndPrimitives.end()) continue;

        std::vector<Primitive*> firstPrimitiveVector = firstPrimitiveVectorIterator->second;
        std::vector<Primitive*> secondPrimitiveVector = secondPrimitiveVectorIterator->second;

        for (Primitive* firstPrimitive : firstPrimitiveVector)
        {
            for (Primitive* secondPrimitive : secondPrimitiveVector)
            {
                generateContact(firstPrimitive, secondPrimitive, allContacts);
            }
        }
    }

    return allContacts;
}

void CollisionDetector::generateContact(Primitive* firstPrimitive, Primitive* secondPrimitive, std::vector<RigidBodyContact>& allContacts)
{
    // Detect primitives types
    
    // If the first primitive is a sphere
    if (dynamic_cast<Sphere*>(firstPrimitive) != nullptr)
    {
        // If the second primitive is a sphere
        if (dynamic_cast<Sphere*>(secondPrimitive) != nullptr)
        {
            sphereAndSphereContact(dynamic_cast<Sphere*>(firstPrimitive), dynamic_cast<Sphere*>(secondPrimitive), allContacts);
        }
    }
}

void CollisionDetector::sphereAndSphereContact(Sphere* firstSphere, Sphere* secondSphere, std::vector<RigidBodyContact>& allContacts)
{
    float distance = Vector3f{ firstSphere->getPosition() - secondSphere->getPosition() }.norm();

    if (distance > firstSphere->getRadius() + secondSphere->getRadius())
    {
        Vector3f vectorToNormalize = Vector3f{ firstSphere->getPosition() - secondSphere->getPosition() }.normalize();

        Vector3f extremitySphereA{ (secondSphere->getPosition() - firstSphere->getPosition()) - (-vectorToNormalize * secondSphere->getRadius()) };
        Vector3f extremitySphereB{ (firstSphere->getPosition() - secondSphere->getPosition()) - (vectorToNormalize * firstSphere->getRadius()) };

        allContacts.emplace_back(firstSphere->getRigidBody(), secondSphere->getRigidBody(), std::abs(firstSphere->getRadius() - (distance - secondSphere->getRadius())), vectorToNormalize, (extremitySphereA + extremitySphereB) / 2.f, 0.95f, 0.95f);
    }
}