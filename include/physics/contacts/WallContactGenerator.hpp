#ifndef MPJVP_WALLCONTACTGENERATOR
#define MPJVP_WALLCONTACTGENERATOR

#include "physics/contacts/ParticleContactGenerator.hpp"
#include "physics/Vector3f.hpp"
#include <vector>

class WallContactGenerator : public ParticleContactGenerator
{
private:
    // Wall properties
    Vector3f m_directionWidth;
    Vector3f m_directionLength;
    float m_length;
    float m_width;
    float m_thickness;
    float m_restitution;
    Vector3f m_centerPosition;

    // Particle affected by the wall
    Particle * m_otherParticle;

public:
    WallContactGenerator() = default;
    WallContactGenerator(Vector3f directionWidth, Vector3f directionLength, float length, float width, float thickness, Vector3f centerPosition, Particle * otherParticle, float restitution = 1.0f);
    virtual ~WallContactGenerator() = default;

    virtual void addContact(std::vector<ParticleContact>& contacts) const;
};

#endif // MPJVPWALLCONTACTGENERATORN