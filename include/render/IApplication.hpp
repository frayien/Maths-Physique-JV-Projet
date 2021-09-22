#ifndef MPJVP_IAPPLICATION
#define MPJVP_IAPPLICATION

#include "render/World.hpp"

class IApplication
{
public:
    IApplication() {}
    virtual ~IApplication() {}

    virtual void init(World & world) = 0;
    virtual void update(World & world, float deltaTime) = 0;
};

#endif // MPJVP_IAPPLICATION
