#include "physics/forces/ParticuleBungeeAnchoredSpring.hpp"

ParticleBungeeAnchoredSpring::ParticleBungeeAnchoredSpring() :
    ParticleAnchoredSpring{}
{

}

ParticleBungeeAnchoredSpring::ParticleBungeeAnchoredSpring(Vector3f anchor, float k, float restLength) : 
    ParticleAnchoredSpring{anchor, k, restLength}
{

}

ParticleBungeeAnchoredSpring::~ParticleBungeeAnchoredSpring()
{
    
}

void ParticleBungeeAnchoredSpring::updateForce(Particle* particle, float duration)
{
    Vector3f distance = particle->getPosition() - m_anchor;
    if (distance.norm() > m_restLength)
    {
        particle->setIsResting(false);

        // Apply force only if the distance is greater than the rest length
        Vector3f force = - m_k * (distance.norm() - m_restLength) * distance.normalize();
        particle->addForce(force);
    }
}