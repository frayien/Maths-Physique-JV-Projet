#include "collision/BoundingVolumeSphere.hpp"

BoundingVolumeSphere::BoundingVolumeSphere(Vector3f position, float radius, RigidBody * rigidbody, Vector3f offset) :
    m_position{position},
    m_radius{radius},
    m_rigidbody{rigidbody},
    m_offset{offset}
{

}

bool BoundingVolumeSphere::intersects(BoundingVolumeSphere* other)
{
    float sqrDistance = (m_position - other->m_position).sqrNorm();
    return sqrDistance <= ((m_radius + other->m_radius) * (m_radius + other->m_radius));
}

void BoundingVolumeSphere::updatePosition()
{
    if (m_rigidbody == nullptr) return;

    m_position = m_rigidbody->getTranformMatrix() * m_offset;
}