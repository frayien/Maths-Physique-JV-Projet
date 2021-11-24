#ifndef FRAYIEN_PARTICLECONTACTRESOLVER
#define FRAYIEN_PARTICLECONTACTRESOLVER

#include <vector>

#include "physics/contacts/Particle/ParticleContact.hpp"

class ParticleContactResolver
{
protected:

    std::size_t m_iteration = 2;

public:
    ParticleContactResolver();
    virtual ~ParticleContactResolver();

    void resolveContacts(std::vector<ParticleContact> & contacts, float deltaTime);
};

#endif // FRAYIEN_PARTICLECONTACTRESOLVER
