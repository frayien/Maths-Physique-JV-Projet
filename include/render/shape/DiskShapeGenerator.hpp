#ifndef MPJVP_DISKSHAPEGENERATOR
#define MPJVP_DISKSHAPEGENERATOR

#include "render/shape/IShapeGenerator.hpp"
#include "render/shape/Transformable.hpp"

#include "render/Color.hpp"

class DiskShapeGenerator : public IShapeGenerator, public Transformable
{
public:
    DiskShapeGenerator(color_t color = Color::GRAY);
    ~DiskShapeGenerator();

    virtual void addShape(std::vector<Shape> & shapes) override;

private:
    color_t m_color;
};

#endif // MPJVP_DISKSHAPEGENERATOR
