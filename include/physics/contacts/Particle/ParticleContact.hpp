#ifndef FRAYIEN_PARTICLECONTACT
#define FRAYIEN_PARTICLECONTACT

#include <array>

#include "physics/Particle.hpp"

class ParticleContact
{
private:
    union
    {
        std::array<Particle*, 2> m_particles;
        struct
        {
            Particle* m_particleA;
            Particle* m_particleB;
        };
    };

    // value between 0 - 1
    // define the elasticness of the collision
    float m_restitution;

    // distance of penetration of the contact
    float m_penetration;

    // contact normal
    Vector3f m_normal;
    
public:
    ParticleContact(Particle* particleA, Particle* particleB, float penetration, Vector3f normal, float restitution = 1.0f);
    virtual ~ParticleContact();

    void resolve(float deltaTime);
    float calculateSeparatingVelocity() const;

    inline std::array<Particle*, 2> & getParticles() { return m_particles; }

    inline void setRestitution(float rest) { m_restitution = rest; }
    inline float getRestitution() { return m_restitution; }

    inline float getPenetration() { return m_penetration; }

    inline Vector3f getNormal() { return m_normal; }

private:
    void resolveVelocity(float deltaTime);
    void resolveInterpenetration();
};

#endif // FRAYIEN_PARTICLECONTACT
