#ifndef MPJVP_APPLICATION
#define MPJVP_APPLICATION

#include <vector>
#include "render/IApplication.hpp"
#include "Particle.hpp"

class Application : public IApplication
{
private:
    static constexpr float PI = glm::pi<float>();
    
    Particle particle;
    std::shared_ptr<Entity> particleRendered;
    std::vector<Vector3f> forceList;

    Vector3f positionInit = {-8, -3, 0};
    Vector3f velocityInit = {0, 4, 7};
    
public:
    virtual void init(World & world) override;
    virtual void update(World & world, float deltaTime) override;
};

#endif // MPJVP_APPLICATION
