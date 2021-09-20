#ifndef FNES_IAPPLICATION
#define FNES_IAPPLICATION

class IApplication
{
public:
    IApplication() {}
    virtual ~IApplication() {}

    virtual void init() = 0;
    virtual void update() = 0;
};

#endif // FNES_IAPPLICATION
