#include "physics/contacts/WallContactGenerator.hpp"

WallContactGenerator::WallContactGenerator(Vector3f normal, float length, float width, float thickness, Particle * wallParticle, Particle * otherParticle) :
    m_normal{normal.normalize()},
    m_length{length},
    m_width{width},
    m_thickness{thickness},
    m_wallParticle{wallParticle},
    m_otherParticle{otherParticle}
{

}

void WallContactGenerator::addContact(std::vector<ParticleContact>& contacts) const
{
    Vector3f vectWallToParticle = m_otherParticle->getPosition() - m_wallParticle->getPosition();
    float normalComponent = vectWallToParticle.dotProduct(m_normal);
    float tangentComponent = (vectWallToParticle - normalComponent * m_normal).norm();

    // TODO : move particle radius inside Particle class
    float particleRadius = 0.2f;
    float normalDistance = normalComponent - particleRadius - m_thickness / 2.0f;

    // TODO : add length and width checking

    if (normalDistance <= 0.0f)
    {
        // Contact with the wall
        contacts.emplace_back(m_otherParticle, m_wallParticle, -normalDistance, m_normal);
    }
}