#include "render/shape/SquareShapeGenerator.hpp"

SquareShapeGenerator::SquareShapeGenerator(color_t color) :
    m_color{color}
{
}

SquareShapeGenerator::~SquareShapeGenerator()
{
}

void SquareShapeGenerator::addShape(std::vector<Shape> & shapes)
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