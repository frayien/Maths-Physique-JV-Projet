#ifndef MPJVP_COLLISIONDETECTOR
#define MPJVP_COLLISIONDETECTOR

#include "collision/octree.hpp"
#include "physics/contacts/RigidBody/RigidBodyContact.hpp"
#include "GameState.hpp"
#include "physics/contacts/RigidBody/Primitive.hpp"
#include "physics/contacts/RigidBody/Sphere.hpp"
#include "physics/contacts/RigidBody/Plane.hpp"
#include "physics/contacts/RigidBody/Box.hpp"

class CollisionDetector
{
private:
    Octree m_octree;

public:
    CollisionDetector();
    virtual ~CollisionDetector() = default;

    std::vector<RigidBodyContact> detectCollisions(GameState& gameState);

private:
    void generateContact(Primitive* firstPrimitive, Primitive* secondPrimitive, std::vector<RigidBodyContact>& allContacts);
    void sphereAndSphereContact(Sphere* firstSphere, Sphere* secondSphere, std::vector<RigidBodyContact>& allContacts);
    void sphereAndPlane(Sphere* sphere, Plane* plane, std::vector<RigidBodyContact>& allContacts);
    void boxAndPlane(Box* box, Plane* plane, std::vector<RigidBodyContact>& allContacts);
};

#endif // MPJVP_COLLISIONDETECTOR
