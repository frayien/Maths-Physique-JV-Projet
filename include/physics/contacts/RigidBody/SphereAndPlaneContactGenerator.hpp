#ifndef MPJVP_SPHEREANDPLANECONTACTGENERATOR
#define MPJVP_SPHEREANDPLANECONTACTGENERATOR

#include "physics/contacts/RigidBody/RigidBodyContactGenerator.hpp"
#include "physics/contacts/RigidBody/Sphere.hpp"
#include "physics/contacts/RigidBody/Plane.hpp"

class SphereAndPlaneContactGenerator : public RigidBodyContactGenerator
{
private:
    Sphere* m_sphere;
    Plane* m_plane;
    
public:
    SphereAndPlaneContactGenerator(Sphere* sphere, Plane* plane);
    virtual ~SphereAndPlaneContactGenerator() = default;

    virtual void addContact(std::vector<RigidBodyContact>& contacts) const override;
};

#endif // MPJVP_SPHEREANDPLANECONTACTGENERATOR
