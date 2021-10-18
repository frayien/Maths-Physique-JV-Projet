#include "physics/ParticleDrag.hpp"

ParticleDrag::ParticleDrag(float k1, float k2) :
    m_k1{k1},
    m_k2{k2}
{

}

ParticleDrag::~ParticleDrag()
{

}

void ParticleDrag::setK1(float k1)
{
    m_k1 = k1;
}

void ParticleDrag::setK2(float k2)
{
    m_k2 = k2;
}

void ParticleDrag::updateForce(Particle* particle, float duration)
{
    Vector3f force = - particle->getVelocity().normalize() * (m_k1 * particle->getVelocity().norm() + m_k2 * particle->getVelocity().norm() * particle->getVelocity().norm());
    particle->addForce(force);
}