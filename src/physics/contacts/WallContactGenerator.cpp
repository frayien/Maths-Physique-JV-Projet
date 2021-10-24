#include "physics/contacts/WallContactGenerator.hpp"

WallContactGenerator::WallContactGenerator(Vector3f directionWidth, Vector3f directionLength, float length, float width, float thickness, Vector3f centerPosition, float restitution) :
    m_directionWidth{directionWidth.normalize()},
    m_directionLength{directionLength.normalize()},
    m_length{length},
    m_width{width},
    m_thickness{thickness},
    m_restitution{restitution},
    m_centerPosition{centerPosition}
{

}

void WallContactGenerator::addContact(std::vector<ParticleContact>& contacts) const
{
    for (Particle * particle : m_particles)
    {
        Vector3f vectWallToParticle = particle->getPosition() - m_centerPosition;
        Vector3f wallNormal = m_directionWidth.crossProduct(m_directionLength).normalize();
        float normalComponentNorm = vectWallToParticle.dotProduct(wallNormal);
        Vector3f tangentComponent = vectWallToParticle - normalComponentNorm * wallNormal;

        // We get the particle's radius and its penetration inside the wall
        float particleRadius = particle->getRadius();
        float penetration = abs(normalComponentNorm) - particleRadius - m_thickness / 2.0f;

        if (penetration < 0.0f && penetration >= -m_thickness / 2.0f && abs(tangentComponent.dotProduct(m_directionLength)) <= m_length / 2.0f && abs(tangentComponent.dotProduct(m_directionWidth)) <= m_width / 2.0f)
        {
            // Contact with the wall

            // Check if the contact is on the front of the wall or on the back
            Vector3f normal = normalComponentNorm < 0.0f ? -wallNormal : wallNormal;

            // Create contact
            contacts.emplace_back(particle, nullptr, -penetration, normal, m_restitution);
        }
    }
}

void WallContactGenerator::removeParticle(Particle * particle)
{
    auto it = std::find(m_particles.begin(), m_particles.end(), particle);
    if (it != m_particles.end())
    {
        m_particles.erase(it);
    }
}