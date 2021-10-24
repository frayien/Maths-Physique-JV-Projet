#ifndef MPJVP_NAIVEPARTICLECONTACTGENERATOR
#define MPJVP_NAIVEPARTICLECONTACTGENERATOR

#include "physics/contacts/ParticleLink.hpp"
#include <vector>

class NaiveParticleContactGenerator : public ParticleLink
{
private:

public:
    NaiveParticleContactGenerator() = default;
    NaiveParticleContactGenerator(Particle* particleOne, Particle* particleTwo);
    virtual ~NaiveParticleContactGenerator() = default;

    void addContact(std::vector<ParticleContact>& contacts) const;
};

#endif // MPJVP_NAIVEPARTICLECONTACTGENERATOR