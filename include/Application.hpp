#ifndef MPJVP_APPLICATION
#define MPJVP_APPLICATION

#include <vector>
#include <map>
#include <functional>

#include "render/IApplication.hpp"
#include "physics/Particle.hpp"
#include "physics/ParticleGravity.hpp"
#include "physics/PhysicsEngine.hpp"

class Application : public IApplication
{
private:
    static constexpr float PI = glm::pi<float>();
    
    std::shared_ptr<BufferedShape> m_particleRendered;

    std::vector<Particle> m_particles;
    ParticleGravity m_particleGravity;
    PhysicsEngine m_physicsEngine;

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

    void setResetMarks(bool resetMarks);
};

#endif // MPJVP_APPLICATION
