#include "physics/contacts/RigidBody/Sphere.hpp"

Sphere::Sphere(RigidBody* rigidBody, Matrix34 offset, float radius) :
    Primitive{rigidBody, offset},
    m_radius{radius}
{

}