#include "physics/forces/ParticleSpring.hpp"

ParticleSpring::ParticleSpring() :
    m_otherParticle{nullptr},
    m_k{0.0f},
    m_restLength{0.0f}
{

}

ParticleSpring::ParticleSpring(Particle * otherParticle, float k, float restLength) :
    m_otherParticle{otherParticle},
    m_k{k},
    m_restLength{restLength}
{

}

ParticleSpring::~ParticleSpring()
{

}

void ParticleSpring::setK(float k)
{
    m_k = k;
}

void ParticleSpring::setRestLength(float restLength)
{
    m_restLength = restLength;
}

void ParticleSpring::setOtherParticle(Particle * other)
{
    m_otherParticle = other;
}

void ParticleSpring::updateForce(Particle* particle, float duration)
{
    particle->setIsResting(false);

    Vector3f distance = particle->getPosition() - m_otherParticle->getPosition();
    Vector3f force = - m_k * (distance.norm() - m_restLength) * distance.normalize();
    particle->addForce(force);
}