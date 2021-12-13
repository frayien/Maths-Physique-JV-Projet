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

// //////////// friends //////////// //

std::ostream& operator<<(std::ostream & out, const RigidBodyContact & c)
{
    out << "[\n" << "\t - Point d'impact : " << c.m_position << std::endl;
    out << "\t - Normale : " << c.m_normal << std::endl;
    out << "\t - Interpenetration : " << c.m_penetration << std::endl;
    out << "]";

    return out;
}