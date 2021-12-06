#ifndef MPJVP_BOXANDPLANECONTACTGENERATOR
#define MPJVP_BOXANDPLANECONTACTGENERATOR

#include "physics/contacts/RigidBody/RigidBodyContactGenerator.hpp"
#include "physics/contacts/RigidBody/Box.hpp"
#include "physics/contacts/RigidBody/Plane.hpp"

class BoxAndPlaneContactGenerator : public RigidBodyContactGenerator
{
private:
    Box* m_box;
    Plane* m_plane;

public:
    BoxAndPlaneContactGenerator(Box* box, Plane* plane);
    virtual ~BoxAndPlaneContactGenerator() = default;

    virtual void addContact(std::vector<RigidBodyContact>& contacts) const override;
};

#endif // MPJVP_BOXANDPLANECONTACTGENERATOR
