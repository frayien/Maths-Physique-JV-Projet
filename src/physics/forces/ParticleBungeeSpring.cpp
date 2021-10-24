#include "physics/forces/ParticleBungeeSpring.hpp"

ParticleBungeeSpring::ParticleBungeeSpring() :
    ParticleSpring{}
{

}

ParticleBungeeSpring::ParticleBungeeSpring(Particle * otherParticle, float k, float restLength) : 
    ParticleSpring{otherParticle, k, restLength}
{

}

ParticleBungeeSpring::~ParticleBungeeSpring()
{
    
}

void ParticleBungeeSpring::updateForce(Particle* particle, float duration)
{
    Vector3f distance = particle->getPosition() - m_otherParticle->getPosition();
    if (distance.norm() > m_restLength)
    {
        particle->setIsResting(false);

        // Apply force only if the distance is greater than the rest length
        Vector3f force = - m_k * (distance.norm() - m_restLength) * distance.normalize();
        particle->addForce(force);
    }
}