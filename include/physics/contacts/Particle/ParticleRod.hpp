#ifndef FRAYIEN_PARTICLEROD
#define FRAYIEN_PARTICLEROD

#include <vector>

#include "physics/contacts/Particle/ParticleLink.hpp"

class ParticleRod : public ParticleLink
{
private:
    float m_lengthConstraint;

public:
    ParticleRod() = default;
    ParticleRod(Particle* particleOne, Particle* particleTwo, float length);
    virtual ~ParticleRod() = default;

    void addContact(std::vector<ParticleContact>& contacts) const;
};

#endif // FRAYIEN_PARTICLEROD