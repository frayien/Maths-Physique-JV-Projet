#include "physics/contacts/ParticleContact.hpp"

ParticleContact::ParticleContact(Particle* particleA, Particle* particleB, float penetration, Vector3f normal, float restitution) :
    m_particles { particleA, particleB },
    m_penetration { penetration },
    m_normal { normal },
    m_restitution { restitution }
{
}

ParticleContact::~ParticleContact()
{
}

void ParticleContact::resolve(float deltaTime)
{
    m_particleA->setIsResting(false);
    if (m_particleB)
    {
        m_particleB->setIsResting(false);
    }

    resolveVelocity(deltaTime);
    resolveInterpenetration();
}

float ParticleContact::calculateSeparatingVelocity() const
{
    Vector3f relativeVelocity = m_particleA->getVelocity();
    if (m_particleB)
    {
        relativeVelocity -= m_particleB->getVelocity();
    }

    return relativeVelocity.dotProduct(m_normal);
}

void ParticleContact::resolveVelocity(float deltaTime)
{
    float sepVelocity = calculateSeparatingVelocity();

    if (sepVelocity > 0.0f)
    {
        return;
    }

    float newSepVelocity = -sepVelocity * m_restitution;

    // /!\ Only consider gravity acceleration for resting contact for the moment

    Vector3f accelVelocity{0.0f, 0.0f, -9.81f};
    if (m_particleB)
    {
        accelVelocity -= Vector3f{0.0f, 0.0f, -9.81f};
    }

    float accelSepVelocity = accelVelocity.dotProduct(m_normal) * deltaTime;

    if (accelSepVelocity < 0.0f)
    {
        newSepVelocity += m_restitution * accelSepVelocity;
        if (newSepVelocity < 0.0f)
        {
            newSepVelocity = 0.0f;
        }
    }

    float deltaVelocity = newSepVelocity - sepVelocity;
    float totalInvMass = m_particleA->getInverseMass();

    if (m_particleB)
    {
        totalInvMass += m_particleB->getInverseMass();
    }

    Vector3f impulse = m_normal * (deltaVelocity / totalInvMass);

    m_particleA->setVelocity(m_particleA->getVelocity() + impulse * m_particleA->getInverseMass());

    if (m_particleB)
    {
        m_particleB->setVelocity(m_particleB->getVelocity() - impulse * m_particleB->getInverseMass());
    }
    else
    {
        // Only check if particle is resting when it's a collision with the scene : m_particleB is nullptr
        if (9.81f * deltaTime >= abs(m_particleA->getVelocity().dotProduct(m_normal)))
        {
            // Velocity caused by gravity : resting
            m_particleA->setIsResting(true);
            m_particleA->addVelocity(-m_normal * m_particleA->getVelocity().dotProduct(m_normal));
        }
    }
}

void ParticleContact::resolveInterpenetration()
{
    if (m_particleB)
    {
        float sumMass = m_particleA->getMass() + m_particleB->getMass();
        Vector3f dPosA =  (m_particleB->getMass() / sumMass) * m_penetration * m_normal;
        Vector3f dPosB = -(m_particleA->getMass() / sumMass) * m_penetration * m_normal;

        m_particleA->translate(dPosA);
        m_particleB->translate(dPosB);
    }
    else
    {
        Vector3f dPosA =  m_penetration * m_normal;
        m_particleA->translate(dPosA);
    }
}