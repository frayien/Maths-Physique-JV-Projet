#ifndef FRAYIEN_RIGIDBODYCONTACTGENERATOR
#define FRAYIEN_RIGIDBODYCONTACTGENERATOR

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

#endif // FRAYIEN_RIGIDBODYCONTACTGENERATOR