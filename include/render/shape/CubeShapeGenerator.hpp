#ifndef MPJVP_CUBESHAPEGENERATOR
#define MPJVP_CUBESHAPEGENERATOR

#include "render/shape/IShapeGenerator.hpp"
#include "render/shape/Transformable.hpp"

#include "render/Color.hpp"

class CubeShapeGenerator : public IShapeGenerator, public Transformable
{
public:
    CubeShapeGenerator(color_t color = Color::GRAY);
    ~CubeShapeGenerator();

    virtual void addShape(std::vector<Shape> & shapes) override;

private:
    color_t m_color;
};

#endif // MPJVP_CUBESHAPEGENERATOR
