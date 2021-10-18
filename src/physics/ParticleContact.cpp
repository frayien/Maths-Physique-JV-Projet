#include "physics/ParticleContact.hpp"

ParticleContact::ParticleContact(Particle* particleA, Particle* particleB, float penetration, Vector3f normal) :
    m_particles { particleA, particleB },
    m_penetration { penetration },
    m_normal { normal }
{
}

ParticleContact::~ParticleContact()
{
}

void ParticleContact::resolve(float deltaTime)
{
    resolveVelocity();
    resolveInterpenetration();
}

float ParticleContact::calculateSeparatingVelocity()
{
    return (m_particles[0]->getVelocity() - m_particles[1]->getVelocity()).dotProduct(m_normal);
}

void ParticleContact::resolveVelocity()
{

}

void ParticleContact::resolveInterpenetration()
{

}