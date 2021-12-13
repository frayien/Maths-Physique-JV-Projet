#ifndef MPJVP_RIGIDBODYCONTACT
#define MPJVP_RIGIDBODYCONTACT

#include "physics/Vector3f.hpp"
#include "physics/RigidBody.hpp"

class RigidBodyContact
{
public:
    RigidBodyContact(RigidBody* rigidBodyA, RigidBody* rigidBodyB, float penetration, Vector3f normal, Vector3f position, float friction, float restitution = 1.0f);
    virtual ~RigidBodyContact();

    void resolve(float deltaTime);

    friend std::ostream& operator<<(std::ostream & out, const RigidBodyContact & c);

private:
    float m_restitution;
    float m_penetration;
    float m_friction;
    Vector3f m_normal;
    Vector3f m_position;

    RigidBody* m_rigidBodyA;
    RigidBody* m_rigidBodyB;
};

#endif // MPJVP_RIGIDBODYCONTACT
