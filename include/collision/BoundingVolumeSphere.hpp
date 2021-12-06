#ifndef MPJVP_BOUNDINGVOLUMESPHERE
#define MPJVP_BOUNDINGVOLUMESPHERE

#include "physics/RigidBody.hpp"
#include "physics/Vector3f.hpp"

class BoundingVolumeSphere
{
private:
    Vector3f m_position;
    float m_radius;
    RigidBody * m_rigidbody;
    Vector3f m_offset;

public:
    BoundingVolumeSphere(Vector3f position, float radius, RigidBody * rigidbody = nullptr, Vector3f offset = {0.0f, 0.0f, 0.0f});
    virtual ~BoundingVolumeSphere() = default;

    bool intersects(BoundingVolumeSphere* other);
    void updatePosition();

    inline Vector3f getPosition() { return m_position; }
    inline float getRadius() { return m_radius; }
};

#endif // MPJVP_BOUNDINGVOLUMESPHERE
