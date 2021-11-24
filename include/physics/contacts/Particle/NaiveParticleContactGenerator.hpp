#ifndef MPJVP_NAIVEPARTICLECONTACTGENERATOR
#define MPJVP_NAIVEPARTICLECONTACTGENERATOR

#include "physics/contacts/Particle/ParticleContactGenerator.hpp"
#include <vector>

class NaiveParticleContactGenerator : public ParticleContactGenerator
{
private:

    std::vector<Particle*> m_particles;
    float getCurrentDistance(Particle* particleOne, Particle* particleTwo) const;

public:
    NaiveParticleContactGenerator() = default;
    NaiveParticleContactGenerator(std::vector<Particle*> particles);
    virtual ~NaiveParticleContactGenerator() = default;

    void addContact(std::vector<ParticleContact>& contacts) const;
};

#endif // MPJVP_NAIVEPARTICLECONTACTGENERATOR