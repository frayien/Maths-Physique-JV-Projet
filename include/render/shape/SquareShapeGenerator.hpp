#ifndef MPJVP_SQUARESHAPEGENERATOR
#define MPJVP_SQUARESHAPEGENERATOR

#include "render/shape/IShapeGenerator.hpp"
#include "render/shape/Transformable.hpp"

class SquareShapeGenerator : public IShapeGenerator, public Transformable
{
public:
    SquareShapeGenerator(glm::vec3 color = { 0.5f, 0.5f, 0.5f });
    ~SquareShapeGenerator();

    virtual void addShape(std::vector<Shape> & shapes) const override;

private:
    glm::vec3 m_color;
};

#endif // MPJVP_SQUARESHAPEGENERATOR
