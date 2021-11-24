#ifndef FRAYIEN_PARTICLECABLE
#define FRAYIEN_PARTICLECABLE

#include <vector>

#include "physics/contacts/Particle/ParticleLink.hpp"

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

    inline void setRestitution(float restitution) { m_restitution = restitution; }
    inline void setMaxLength(float maxLength) { m_maxLength = maxLength; }
};

#endif // FRAYIEN_PARTICLECABLE