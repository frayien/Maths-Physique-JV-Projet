#include "physics/contacts/RigidBody/SphereAndPlaneContactGenerator.hpp"

SphereAndPlaneContactGenerator::SphereAndPlaneContactGenerator(Sphere* sphere, Plane* plane) :
    m_sphere(sphere),
    m_plane(plane)
{
}

void SphereAndPlaneContactGenerator::addContact(std::vector<RigidBodyContact>& contacts) const
{
    float distance = m_plane->getNormal().dotProduct(m_sphere->getPosition() - m_plane->getPosition());

    if(distance < m_sphere->getRadius() + m_plane->getThickness()/2.f)
    {
        float penetration = m_sphere->getRadius() + m_plane->getThickness()/2.f - distance;
        contacts.emplace_back(m_sphere->getRigidBody(), m_plane->getRigidBody(), penetration, m_plane->getNormal(), m_plane->getPosition(), 0.1f, 0.99f);
    }
}