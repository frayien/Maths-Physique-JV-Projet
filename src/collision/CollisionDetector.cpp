#include "collision/CollisionDetector.hpp"
#include "collision/BoundingVolumeSphere.hpp"

CollisionDetector::CollisionDetector() :
    // Initialize octree with large dimensions (maybe change that later)
    m_octree{Vector3f{0.0f, 0.0f, 0.0f}, 10000.0f}
{

}

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

        if (firstPrimitiveVectorIterator == linksBetweenBoundingVolumesAndPrimitives.end() || secondPrimitiveVectorIterator == linksBetweenBoundingVolumesAndPrimitives.end()) continue;

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

        // If the second primitive is a plane
        if (dynamic_cast<Plane*>(secondPrimitive) != nullptr)
        {
            sphereAndPlane(dynamic_cast<Sphere*>(firstPrimitive), dynamic_cast<Plane*>(secondPrimitive), allContacts);
        }
    }
    // If the first primitive is a box
    else if (dynamic_cast<Box*>(firstPrimitive) != nullptr)
    {
        // If the second primitive is a plane
        if (dynamic_cast<Plane*>(secondPrimitive) != nullptr)
        {
            boxAndPlane(dynamic_cast<Box*>(firstPrimitive), dynamic_cast<Plane*>(secondPrimitive), allContacts);
        }
    }
    // If the first primitive is a plane
    else if (dynamic_cast<Plane*>(firstPrimitive) != nullptr)
    {
        // If the second primitive is a box
        if (dynamic_cast<Box*>(secondPrimitive) != nullptr)
        {
            boxAndPlane(dynamic_cast<Box*>(secondPrimitive), dynamic_cast<Plane*>(firstPrimitive), allContacts);
        }

        // If the second primitive is a sphere
        if (dynamic_cast<Sphere*>(secondPrimitive) != nullptr)
        {
            sphereAndPlane(dynamic_cast<Sphere*>(secondPrimitive), dynamic_cast<Plane*>(firstPrimitive), allContacts);
        }
    }
}

void CollisionDetector::sphereAndSphereContact(Sphere* firstSphere, Sphere* secondSphere, std::vector<RigidBodyContact>& allContacts)
{
    float distance = Vector3f{ firstSphere->getPosition() - secondSphere->getPosition() }.norm();

    if (distance < firstSphere->getRadius() + secondSphere->getRadius())
    {
        Vector3f normale = Vector3f{ firstSphere->getPosition() - secondSphere->getPosition() }.normalize();

        Vector3f extremityFirstSphere{firstSphere->getPosition() - normale * firstSphere->getRadius()};
        Vector3f extremitySecondSphere{secondSphere->getPosition() + normale * secondSphere->getRadius()};

        allContacts.emplace_back(firstSphere->getRigidBody(), secondSphere->getRigidBody(), std::abs(firstSphere->getRadius() - (distance - secondSphere->getRadius())), normale, (extremityFirstSphere + extremitySecondSphere) / 2.f, 0.95f, 0.95f);
    }
}

void CollisionDetector::sphereAndPlane(Sphere* sphere, Plane* plane, std::vector<RigidBodyContact>& allContacts)
{
    float distance = plane->getNormal().dotProduct(sphere->getPosition() - plane->getPosition());

    if(glm::abs(distance) < sphere->getRadius() + plane->getThickness()/2.f)
    {
        float penetration = sphere->getRadius() + plane->getThickness()/2.f - distance;
        Vector3f position = sphere->getPosition() - plane->getNormal() * (sphere->getRadius() - penetration / 2.0f);
        allContacts.emplace_back(sphere->getRigidBody(), plane->getRigidBody(), penetration, plane->getNormal(), position, 0.1f, 0.99f);
    }
}

void CollisionDetector::boxAndPlane(Box* box, Plane* plane, std::vector<RigidBodyContact>& allContacts)
{
    std::array<Vector3f, 8> vertices
    {
        Vector3f{ -box->getHalfSize().getX(), -box->getHalfSize().getY(), -box->getHalfSize().getZ() },
        Vector3f{ -box->getHalfSize().getX(), -box->getHalfSize().getY(), +box->getHalfSize().getZ() },
        Vector3f{ -box->getHalfSize().getX(), +box->getHalfSize().getY(), -box->getHalfSize().getZ() },
        Vector3f{ -box->getHalfSize().getX(), +box->getHalfSize().getY(), +box->getHalfSize().getZ() },
        Vector3f{ +box->getHalfSize().getX(), -box->getHalfSize().getY(), -box->getHalfSize().getZ() },
        Vector3f{ +box->getHalfSize().getX(), -box->getHalfSize().getY(), +box->getHalfSize().getZ() },
        Vector3f{ +box->getHalfSize().getX(), +box->getHalfSize().getY(), -box->getHalfSize().getZ() },
        Vector3f{ +box->getHalfSize().getX(), +box->getHalfSize().getY(), +box->getHalfSize().getZ() }
    };

    Vector3f centerPos = box->getPosition();

    for(std::size_t i = 0; i < vertices.size(); ++i)
    {
        vertices[i] = centerPos + box->getRotation() * vertices[i];
    }

    for(auto & vertex : vertices)
    {
        float distance = plane->getNormal().dotProduct(vertex - plane->getPosition());

        if(glm::abs(distance) < plane->getThickness()/2.f)
        {
            float penetration = plane->getThickness()/2.f - distance;
            Vector3f position = vertex + (penetration/2.f) * plane->getNormal();
            allContacts.emplace_back(box->getRigidBody(), plane->getRigidBody(), penetration, plane->getNormal(), position, 0.1f, 0.99f);
        }
    }
}