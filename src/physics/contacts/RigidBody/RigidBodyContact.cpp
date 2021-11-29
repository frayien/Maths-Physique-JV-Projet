#include "physics/contacts/RigidBody/RigidBodyContact.hpp"

RigidBodyContact::RigidBodyContact(RigidBody* rigidBodyA, RigidBody* rigidBodyB, float penetration, Vector3f normal, Vector3f position, float friction, float restitution) :
    m_rigidBodyA(rigidBodyA),
    m_rigidBodyB(rigidBodyB),
    m_penetration(penetration),
    m_normal(normal),
    m_position(position),
    m_friction(friction),
    m_restitution(restitution)
{
}

RigidBodyContact::~RigidBodyContact()
{
}

void RigidBodyContact::resolve(float deltaTime)
{
    
}