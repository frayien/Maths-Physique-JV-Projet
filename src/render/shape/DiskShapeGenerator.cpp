#include "render/shape/DiskShapeGenerator.hpp"

DiskShapeGenerator::DiskShapeGenerator(color_t color) :
    m_color{color}
{
}

DiskShapeGenerator::~DiskShapeGenerator()
{
}

void DiskShapeGenerator::addShape(std::vector<Shape> & shapes)
{
    const size_t VERTEX_N = 50;
    std::vector<Vertex> vertices = { {{0.0f, 0.0f, 0.0f}, m_color, {0.0f, 0.0f, 1.0f}} };
    std::vector<uint32_t> indices;

    for(size_t i = 0; i<VERTEX_N; ++i)
    {
        float angle = static_cast<float>(i) / static_cast<float>(VERTEX_N) * 2.0f * glm::pi<float>();

        vertices.push_back({{ glm::cos(angle), glm::sin(angle), 0.0f}, m_color, {0.0f, 0.0f, 1.0f}});
        indices.push_back(0);
        indices.push_back(i+1);
        indices.push_back(i+2);
    }
    indices.back() = 1;

    shapes.push_back({vertices, indices, getTransform()});
}
