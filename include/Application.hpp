#ifndef MPJVP_APPLICATION
#define MPJVP_APPLICATION

#include <vector>
#include <map>
#include <functional>

#include "render/IApplication.hpp"
#include "physics/Particle.hpp"
#include "physics/ParticleForceRegistry.hpp"
#include "physics/ParticleGravity.hpp"

class Application : public IApplication
{
private:
    static constexpr float PI = glm::pi<float>();
    
    std::shared_ptr<BufferedShape> m_particleRendered;

    Particle m_particle;
    ParticleGravity m_particleGravity;

    ParticleForceRegistry m_particleRegistry;

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

    inline Particle & getParticle() { return m_particle; }

    void setPositionInit(Vector3f positionInit);
    void setVelocityInit(Vector3f velocityInit);

    void setResetMarks(bool resetMarks);
};

#endif // MPJVP_APPLICATION
