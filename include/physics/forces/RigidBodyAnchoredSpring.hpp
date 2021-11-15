#ifndef MPJVP_RIGIDBODYANCHOREDSPRING
#define MPJVP_RIGIDBODYANCHOREDSPRING

#include "physics/forces/RigidBodyForceGenerator.hpp"
#include "physics/Vector3f.hpp"

class RigidBodyAnchoredSpring : public RigidBodyForceGenerator
{
private:
    Vector3f m_bodyAnchor; // Body anchor point in local coordinate

    Vector3f m_anchor; // Anchor point in world coordinate

    // Spring parameters
    float m_k;
    float m_restLength;

public:
    RigidBodyAnchoredSpring(Vector3f bodyAnchor, Vector3f anchor, float k, float restLength);
    virtual ~RigidBodyAnchoredSpring();

    void updateForce(RigidBody* rigidBody, float duration);
};

#endif // MPJVP_RIGIDBODYANCHOREDSPRING