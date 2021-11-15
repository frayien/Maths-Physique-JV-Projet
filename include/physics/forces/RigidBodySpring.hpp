#ifndef MPJVP_RIGIDBODYSPRING
#define MPJVP_RIGIDBODYSPRING

#include "physics/forces/RigidBodyForceGenerator.hpp"
#include "physics/Vector3f.hpp"

class RigidBodySpring : public RigidBodyForceGenerator
{
private:
    Vector3f m_bodyAnchor; // Body asnchor point in local coordinate

    RigidBody * m_otherRigidBody;
    Vector3f m_otherBodyAnchor; // Other body anchor point in other's local coordinate

    // Spring parameters
    float m_k;
    float m_restLength;

public:
    RigidBodySpring(Vector3f bodyAnchor, RigidBody * otherRigidBody, Vector3f otherBodyAnchor, float k, float restLength);
    virtual ~RigidBodySpring();

    void updateForce(RigidBody* rigidBody, float duration);

};

#endif // MPJVP_RIGIDBODYSPRING