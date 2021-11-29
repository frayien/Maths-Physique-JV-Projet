#ifndef MPJVP_SPHEREANDSPHERECONTACT
#define MPJVP_SPHEREANDSPHERECONTACT

#include <vector>
#include <array>

#include "physics/contacts/RigidBody/RigidBodyContactGenerator.hpp"
#include "physics/contacts/RigidBody/Sphere.hpp"

class SphereAndSphereContact : public RigidBodyContactGenerator
{
private:
    std::array<Sphere*, 2> m_spheres;
public:
    SphereAndSphereContact() = default;
    virtual ~SphereAndSphereContact() = default;

    virtual void addContact(std::vector<RigidBodyContact>& contacts) const;
};

#endif // MPJVP_SPHEREANDSPHERECONTACT
