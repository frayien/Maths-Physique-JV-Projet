#ifndef MPJVP_RIGIDBODYCONTACT
#define MPJVP_RIGIDBODYCONTACT

#include "physics/Vector3f.hpp"
#include "physics/RigidBody.hpp"
#include "physics/Matrix33.hpp"

class RigidBodyContact
{
public:
    RigidBodyContact(RigidBody* rigidBodyA, RigidBody* rigidBodyB, float penetration, Vector3f normal, Vector3f position, float friction, float restitution = 1.0f);
    virtual ~RigidBodyContact();

    void calculateInternals();
    void applyPositionChange();
    void applyVelocityChange();

    inline Vector3f getPosition() { return m_position; }
    inline Vector3f getNormal() { return m_normal; }
    inline float getPenetration() { return m_penetration; }
    inline std::array<float, 2> getLinearMove() { return m_linearMove; }
    inline std::array<float, 2> getAngularMove() { return m_angularMove; }
    inline std::array<Vector3f, 2> getRotationAmount() { return m_rotationAmount; }
    inline std::array<RigidBody*, 2> getRigidbodies() { return m_rigidbodies; }
    inline std::array<Vector3f, 2> getRelativeContactPosition() { return m_relativeContactPositions; }

    inline void setPenetration(float penetration) { m_penetration = penetration; }

    friend std::ostream& operator<<(std::ostream & out, const RigidBodyContact & c);

private:
    float m_restitution;
    float m_penetration;
    float m_friction;
    Vector3f m_normal;
    Vector3f m_position;

    std::array<RigidBody*, 2> m_rigidbodies;

    // Contact resolution data
    Matrix33 m_contactToWorld;
    std::array<Vector3f, 2> m_relativeContactPositions;
    Vector3f m_contactVelocity;
    float m_desiredDeltaVelocity;
    std::array<float, 2> m_angularInertia;
    std::array<float, 2> m_lineraInertia;
    std::array<float, 2> m_linearMove;
    std::array<float, 2> m_angularMove;
    std::array<Vector3f, 2> m_rotationAmount;

    void calculateContactBasis();
    void swapRigidbodies();
    Vector3f calculateLocalVelocity(int rigidbodyIndex);
    void calculateDesiredDeltaVelocity();
};

#endif // MPJVP_RIGIDBODYCONTACT
