#ifndef MPJVP_TESTAPPLICATION
#define MPJVP_TESTAPPLICATION

#include <map>
#include <functional>

#include "render/IApplication.hpp"

class TestApplication : public IApplication
{
private:
    static constexpr float PI = glm::pi<float>();

    std::shared_ptr<Shape> platform;
    std::shared_ptr<Shape> cube;
    std::shared_ptr<Shape> sunShape;

public:
    virtual void init(World & world) override;
    virtual void update(World & world, float deltaTime) override;

    void updateCamera(World & world, float deltaTime);
};


#endif // MPJVP_TESTAPPLICATION
