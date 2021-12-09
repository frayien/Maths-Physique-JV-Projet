#include "physics/contacts/RigidBody/Box.hpp"

Box::Box(RigidBody* rigidBody, Matrix34 offset, Vector3f halfSize) :
    Primitive{rigidBody, offset},
    m_halfSize{halfSize}
{

}