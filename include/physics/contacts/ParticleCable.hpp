#ifndef FRAYIEN_PARTICLECABLE
#define FRAYIEN_PARTICLECABLE

#include <vector>

#include "physics/contacts/ParticleLink.hpp"

class ParticleCable : public ParticleLink
{
private:
    float m_maxLength;
    float m_restitution;

public:
    ParticleCable() = default;
    ParticleCable(Particle* particleOne, Particle* particleTwo, float m_maxLength, float m_restitution);
    virtual ~ParticleCable() = default;

    void addContact(std::vector<ParticleContact>& contacts) const;
};

#endif // FRAYIEN_PARTICLECABLE