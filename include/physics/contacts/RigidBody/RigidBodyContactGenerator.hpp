#ifndef MPJVP_RIGIDBODYCONTACTGENERATOR
#define MPJVP_RIGIDBODYCONTACTGENERATOR

#include <vector>

#include "physics/contacts/RigidBody/RigidBodyContact.hpp"

class RigidBodyContactGenerator
{
private:

public:
    RigidBodyContactGenerator() = default;
    virtual ~RigidBodyContactGenerator() = default;

    virtual void addContact(std::vector<RigidBodyContact>& contacts) const = 0;
};

#endif // MPJVP_RIGIDBODYCONTACTGENERATOR
