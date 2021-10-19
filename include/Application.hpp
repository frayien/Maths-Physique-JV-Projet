#ifndef FRAYIEN_APPLICATION
#define FRAYIEN_APPLICATION

#include <vector>
#include <map>
#include <functional>

#include "render/IApplication.hpp"
#include "physics/Particle.hpp"
#include "physics/ParticleGravity.hpp"
#include "physics/PhysicsEngine.hpp"
#include "physics/ParticleAnchoredSpring.hpp"
#include "physics/ParticleDrag.hpp"

class Application : public IApplication
{
private:
    static constexpr float PI = glm::pi<float>();
    
    std::vector<std::shared_ptr<BufferedShape>> m_particleShapes;

    std::vector<Particle> m_particles;
    ParticleGravity m_particleGravity;
    PhysicsEngine m_physicsEngine;
    ParticleAnchoredSpring m_particleAnchoredSpring;
    ParticleDrag m_particleDrag;

    Vector3f m_positionInit = {-8, -3, 0};
    Vector3f m_velocityInit = {0, 4, 7};

    int m_countTimeStepMarks = 0;
    int m_countTimeStepMarksMax = 15;
    std::vector<std::shared_ptr<BufferedShape>> m_marks{};
    bool m_resetMarks = false;
    
public:
    virtual void init(World & world) override;
    virtual void update(World & world, float deltaTime) override;

    void updateCamera(World & world, float deltaTime);

    inline Particle & getParticle(int index) { return m_particles[index]; }

    void setPositionInit(Vector3f positionInit);
    void setVelocityInit(Vector3f velocityInit);

    void applyParticleAnchoredSpringSettings(Vector3f anchorPosition, float springStiffness, float springRestLength);
    void applyParticleDragSettings(float k1, float k2);

    void setResetMarks(bool resetMarks);
};

#endif // FRAYIEN_APPLICATION
