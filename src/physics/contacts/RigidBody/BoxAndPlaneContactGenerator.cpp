#include "physics/contacts/RigidBody/BoxAndPlaneContactGenerator.hpp"


BoxAndPlaneContactGenerator::BoxAndPlaneContactGenerator(Box* box, Plane* plane) :
    m_box(box),
    m_plane(plane)
{
}

void BoxAndPlaneContactGenerator::addContact(std::vector<RigidBodyContact>& contacts) const
{
    std::array<Vector3f, 8> vertices
    {
        Vector3f{ -m_box->getHalfSize().getX(), -m_box->getHalfSize().getY(), -m_box->getHalfSize().getZ() },
        Vector3f{ -m_box->getHalfSize().getX(), -m_box->getHalfSize().getY(), +m_box->getHalfSize().getZ() },
        Vector3f{ -m_box->getHalfSize().getX(), +m_box->getHalfSize().getY(), -m_box->getHalfSize().getZ() },
        Vector3f{ -m_box->getHalfSize().getX(), +m_box->getHalfSize().getY(), +m_box->getHalfSize().getZ() },
        Vector3f{ +m_box->getHalfSize().getX(), -m_box->getHalfSize().getY(), -m_box->getHalfSize().getZ() },
        Vector3f{ +m_box->getHalfSize().getX(), -m_box->getHalfSize().getY(), +m_box->getHalfSize().getZ() },
        Vector3f{ +m_box->getHalfSize().getX(), +m_box->getHalfSize().getY(), -m_box->getHalfSize().getZ() },
        Vector3f{ +m_box->getHalfSize().getX(), +m_box->getHalfSize().getY(), +m_box->getHalfSize().getZ() }
    };

    Vector3f centerPos = m_box->getPosition();

    for(std::size_t i = 0; i < vertices.size(); ++i)
    {
        vertices[i] = centerPos + m_box->getOffset().extractMatrix33() * vertices[i];
    }

    for(auto & vertex : vertices)
    {
        float distance = m_plane->getNormal().dotProduct(vertex - m_plane->getPosition());

        if(distance < m_plane->getThickness()/2.f)
        {
            float penetration = m_plane->getThickness()/2.f - distance;
            Vector3f position = vertex + (penetration/2.f) * m_plane->getNormal();
            contacts.emplace_back(m_box->getRigidBody(), m_plane->getRigidBody(), penetration, m_plane->getNormal(), position, 0.1f, 0.99f);
        }
    }
}