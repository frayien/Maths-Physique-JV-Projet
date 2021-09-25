#ifndef MPJVP_APPLICATION
#define MPJVP_APPLICATION

#include "render/IApplication.hpp"

class Application : public IApplication
{
private:
    static constexpr float PI = glm::pi<float>();
    
public:
    virtual void init(World & world) override;
    virtual void update(World & world, float deltaTime) override;
};

#endif // MPJVP_APPLICATION
