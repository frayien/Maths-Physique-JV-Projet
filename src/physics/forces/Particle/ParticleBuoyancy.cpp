#include "physics/forces/Particle/ParticleBuoyancy.hpp"

ParticleBuoyancy::ParticleBuoyancy(float maxDepth, float volume, float waterHeight, float liquidDensity = 1.0f) :
    m_maxDepth{maxDepth},
    m_volume{volume},
    m_waterHeight{waterHeight},
    m_liquidDensity{liquidDensity}
{

}

void ParticleBuoyancy::updateForce(Particle* particle, float duration)
{
    Vector3f buoyancyForce{0.0f, 0.0f, 0.0f};

    float submergedQuantity = (particle->getPosition().getZ() - m_waterHeight - m_maxDepth) / 2.0f * m_maxDepth;

    if (submergedQuantity <= 0.0f)
    {
        // Not submerged at all
        // No force is applied
    }
    else if (submergedQuantity >= 1.0f)
    {
        // Totally submerged
        particle->setIsResting(false);

        buoyancyForce.setZ(m_volume * m_liquidDensity);
    }
    else
    {
        // Partially submerged
        particle->setIsResting(false);
        
        buoyancyForce.setZ(submergedQuantity * m_volume * m_liquidDensity);
    }

    particle->addForce(buoyancyForce);
}