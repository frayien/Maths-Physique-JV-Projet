#ifndef MPJVP_WALLCONTACTGENERATOR
#define MPJVP_WALLCONTACTGENERATOR

#include "physics/contacts/ParticleContactGenerator.hpp"
#include "physics/Vector3f.hpp"
#include <vector>

class WallContactGenerator : public ParticleContactGenerator
{
private:
    // Wall properties
    Vector3f m_normal;
    float m_length;
    float m_width;
    float m_thickness;
    Particle * m_wallParticle;

    // Particle affected by the wall
    Particle * m_otherParticle;

public:
    WallContactGenerator() = default;
    WallContactGenerator(Vector3f normal, float length, float width, float thickness, Particle * wallParticle, Particle * otherParticle);
    virtual ~WallContactGenerator() = default;

    virtual void addContact(std::vector<ParticleContact>& contacts) const;
};

#endif // MPJVPWALLCONTACTGENERATORN