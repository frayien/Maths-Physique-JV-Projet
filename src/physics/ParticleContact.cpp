#include "physics/ParticleContact.hpp"

ParticleContact::ParticleContact(Particle* particleA, Particle* particleB) :
    m_particles { particleA, particleB }
{
    
}

ParticleContact::~ParticleContact()
{
}

void resolve(float deltaTime)
{
    resolveVelocity();
    resolveInterpenetration();
}

float calculateSeparatingVelocity()
{

}

void resolveVelocity()
{

}

void resolveInterpenetration()
{

}