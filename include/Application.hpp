#ifndef MPJVP_APPLICATION
#define MPJVP_APPLICATION

#include <vector>
#include <map>
#include <functional>

#include "render/IApplication.hpp"
#include "Particle.hpp"

class Application : public IApplication
{
private:
    static constexpr float PI = glm::pi<float>();
    
    Particle particle;
    std::shared_ptr<BufferedShape> particleRendered;

    Vector3f positionInit = {-8, -3, 0};
    Vector3f velocityInit = {0, 4, 7};
    
public:
    virtual void init(World & world) override;
    virtual void update(World & world, float deltaTime) override;

    void updateCamera(World & world, float deltaTime);

    inline Particle & getParticle() { return particle; }

    void setPositionInit(Vector3f positionInit);
    void setVelocityInit(Vector3f velocityInit);
};

#endif // MPJVP_APPLICATION
