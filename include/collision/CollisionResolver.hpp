#ifndef MPJVP_COLLISIONRESOLVER
#define MPJVP_COLLISIONRESOLVER

#include <vector>
#include "physics/contacts/RigidBody/RigidBodyContact.hpp"
#include "physics/Vector3f.hpp"

class CollisionResolver
{
private:

public:
    CollisionResolver() = default;
    virtual ~CollisionResolver() = default;

    void resolveCollisions(std::vector<RigidBodyContact> collisions);

private:
    void updatePenetrations(std::vector<RigidBodyContact> collisions, int indexMaxPenetration);

};

#endif // MPJVP_COLLISIONRESOLVER
