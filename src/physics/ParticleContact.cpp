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

float ParticleContact::calculateSeparatingVelocity() const
{
    return (m_particleA->getVelocity() - m_particleB->getVelocity()).dotProduct(m_normal);
}

void ParticleContact::resolveVelocity()
{
    float sumInvMass = m_particleA->getInverseMass() + m_particleB->getInverseMass();
    float sepVel = calculateSeparatingVelocity();
    float k = (m_restitution + 1.0f) * sepVel / sumInvMass;

    m_particleA->addVelocity(-k*m_particleA->getInverseMass() * m_normal);
    m_particleB->addVelocity( k*m_particleB->getInverseMass() * m_normal);
}

void ParticleContact::resolveInterpenetration()
{
    float sumMass = m_particleA->getMass() + m_particleB->getMass();
    Vector3f dPosA =  (m_particleB->getMass() / sumMass) * m_penetration * m_normal;
    Vector3f dPosB = -(m_particleA->getMass() / sumMass) * m_penetration * m_normal;

    m_particleA->translate(dPosA);
    m_particleB->translate(dPosB);
}