#ifndef FNES_IAPPLICATION
#define FNES_IAPPLICATION

#include "render/World.hpp"

class IApplication
{
public:
    IApplication() {}
    virtual ~IApplication() {}

    virtual void init(World & world) = 0;
    virtual void update(World & world) = 0;
};

#endif // FNES_IAPPLICATION
