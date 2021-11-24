#include "physics/contacts/Particle/ParticleContactResolver.hpp"

ParticleContactResolver::ParticleContactResolver()
{
}

ParticleContactResolver::~ParticleContactResolver()
{
}

void ParticleContactResolver::resolveContacts(std::vector<ParticleContact> & contacts, float deltaTime)
{
    m_iteration = contacts.size() * 2;

    for(std::size_t i = 0; i < m_iteration; ++i)
    {
        float minVel = 1.0f; // only negative velocity are relevent, we can init with any positive number here
        ParticleContact* minContact = nullptr;

        // find the most important contact (min separating speed)
        for(ParticleContact & contact : contacts)
        {
            float vel = contact.calculateSeparatingVelocity();
            if(vel < minVel)
            {
                minVel = vel;
                minContact = &contact;
            }
        }

        // no contact found
        if(minVel > 0)
        {
            break;
        }

        minContact->resolve(deltaTime);
    }
}