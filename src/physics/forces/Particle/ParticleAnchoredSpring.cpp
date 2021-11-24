#include "physics/forces/Particle/ParticleAnchoredSpring.hpp"

ParticleAnchoredSpring::ParticleAnchoredSpring() :
    m_anchor{0.0f, 0.0f, 0.0f},
    m_k{0.0f},
    m_restLength{0.0f}
{

}

ParticleAnchoredSpring::ParticleAnchoredSpring(Vector3f anchor, float k, float restLength) :
    m_anchor{anchor},
    m_k{k},
    m_restLength{restLength}
{

}

ParticleAnchoredSpring::~ParticleAnchoredSpring()
{

}

void ParticleAnchoredSpring::setK(float k)
{
    m_k = k;
}

void ParticleAnchoredSpring::setRestLength(float restLength)
{
    m_restLength = restLength;
}

void ParticleAnchoredSpring::setAnchor(Vector3f anchor)
{
    m_anchor = anchor;
}

void ParticleAnchoredSpring::updateForce(Particle* particle, float duration)
{
    particle->setIsResting(false);

    Vector3f distance = particle->getPosition() - m_anchor;
    Vector3f force = - m_k * (distance.norm() - m_restLength) * distance.normalize();
    particle->addForce(force);
}