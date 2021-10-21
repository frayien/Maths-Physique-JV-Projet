#ifndef MPJVP_SQUARESHAPEGENERATOR
#define MPJVP_SQUARESHAPEGENERATOR

#include "render/shape/IShapeGenerator.hpp"
#include "render/shape/Transformable.hpp"

class SquareShapeGenrator : public IShapeGenerator, public Transformable
{
public:
    SquareShapeGenrator(glm::vec3 color = { 0.5f, 0.5f, 0.5f });
    ~SquareShapeGenrator();

    virtual void addShape(std::vector<Shape> & shapes) const override;

private:
    glm::vec3 m_color;
};

SquareShapeGenrator::SquareShapeGenrator(glm::vec3 color) :
    m_color{color}
{
}

SquareShapeGenrator::~SquareShapeGenrator()
{
}

void SquareShapeGenrator::addShape(std::vector<Shape> & shapes) const
{
    std::vector<Vertex> vertices = 
    {
        {{-1.0f, -1.0f, 0.0f}, m_color, {0.0f, 0.0f, 1.0f}},
        {{ 1.0f, -1.0f, 0.0f}, m_color, {0.0f, 0.0f, 1.0f}},
        {{ 1.0f,  1.0f, 0.0f}, m_color, {0.0f, 0.0f, 1.0f}},
        {{-1.0f,  1.0f, 0.0f}, m_color, {0.0f, 0.0f, 1.0f}},
    };
    std::vector<uint32_t> indices = 
    {
        0,1,2,2,3,0,
    };

    shapes.push_back({vertices, indices, getTransform()});
}

#endif // MPJVP_SQUARESHAPEGENERATOR
