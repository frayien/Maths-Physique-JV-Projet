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

    // Particles affected by the wall
    std::vector<Particle *> m_particles;

public:
    WallContactGenerator() = default;
    WallContactGenerator(Vector3f directionWidth, Vector3f directionLength, float length, float width, float thickness, Vector3f centerPosition, float restitution = 1.0f);
    virtual ~WallContactGenerator() = default;

    virtual void addContact(std::vector<ParticleContact>& contacts) const;

    inline void setLength(float length) { m_length = length; }
    inline void setWidth(float width) { m_width = width; }
    inline void setThickness(float thickness) { m_thickness = thickness; }
    inline void setRestitution(float restitution) { m_restitution = restitution; }
    inline void setCenterPosition(Vector3f centerPosition) { m_centerPosition = centerPosition; }

    void addParticle(Particle * particle) { m_particles.push_back(particle); }
    void removeParticle(Particle * particle);
};

#endif // MPJVPWALLCONTACTGENERATORN