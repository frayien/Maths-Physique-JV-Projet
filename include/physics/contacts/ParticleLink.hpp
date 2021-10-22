#ifndef FRAYIEN_PARTICLELINK
#define FRAYIEN_PARTICLELINK

#include <array>
#include <cmath>
#include "physics/contacts/ParticleContactGenerator.hpp"

class ParticleLink : public ParticleContactGenerator
{
protected:
    std::array<Particle*, 2> m_particles;

private:

public:
    ParticleLink() = default;
    ParticleLink(Particle* particleOne, Particle* particleTwo);
    virtual ~ParticleLink() = default;

    float getCurrentLength() const;

    virtual void addContact(std::vector<ParticleContact>& contacts) const = 0;
};

#endif // FRAYIEN_PARTICLELINK