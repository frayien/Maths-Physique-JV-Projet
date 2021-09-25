#ifndef MPJVP_APPLICATION
#define MPJVP_APPLICATION

#include <vector>
#include "render/IApplication.hpp"
#include "Particle.hpp"

class Application : public IApplication
{
private:
    static constexpr float PI = glm::pi<float>();
    Particle * particle;
    std::shared_ptr<Entity> particleRendered;
    const float TIMESTEP = 1.0f;
    float lag = 0.0f;
    std::vector<Vector3f> forceList;
    
public:
    virtual void init(World & world) override;
    virtual void update(World & world, float deltaTime) override;
};

#endif // MPJVP_APPLICATION
