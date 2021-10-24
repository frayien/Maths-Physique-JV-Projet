#ifndef MPJVP_SQUARESHAPEGENERATOR
#define MPJVP_SQUARESHAPEGENERATOR

#include "render/shape/IShapeGenerator.hpp"
#include "render/shape/Transformable.hpp"

#include "render/Color.hpp"

class SquareShapeGenerator : public IShapeGenerator, public Transformable
{
public:
    SquareShapeGenerator(color_t color = Color::GRAY);
    ~SquareShapeGenerator();

    virtual void addShape(std::vector<Shape> & shapes) override;

private:
    color_t m_color;
};

#endif // MPJVP_SQUARESHAPEGENERATOR
