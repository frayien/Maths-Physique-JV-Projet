#include "physics/contacts/RigidBody/Plane.hpp"

Plane::Plane(RigidBody* rigidBody, Matrix34 offset, Vector3f normal, float thickness) :
    Primitive{rigidBody, offset},
    m_normal{normal},
    m_thickness{thickness}
{

}