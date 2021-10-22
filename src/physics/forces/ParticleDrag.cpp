#include "physics/forces/ParticleDrag.hpp"

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
    float vel = particle->getVelocity().norm();
    Vector3f force = - particle->getVelocity().normalize() * (m_k1 * vel + m_k2 * vel * vel);
    particle->addForce(force);
}