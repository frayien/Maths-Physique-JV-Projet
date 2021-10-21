#include "render/shape/CubeShapeGenerator.hpp"

CubeShapeGenerator::CubeShapeGenerator(glm::vec3 color) :
    m_color{color}
{
}

CubeShapeGenerator::~CubeShapeGenerator()
{
}

void CubeShapeGenerator::addShape(std::vector<Shape> & shapes) const
{
    std::vector<Vertex> vertices = 
    {
        {{ 1.0f, -1.0f,  1.0f}, m_color, { 1.0f, 0.0f, 0.0f}}, // x+
        {{ 1.0f, -1.0f, -1.0f}, m_color, { 1.0f, 0.0f, 0.0f}},
        {{ 1.0f,  1.0f, -1.0f}, m_color, { 1.0f, 0.0f, 0.0f}},
        {{ 1.0f,  1.0f,  1.0f}, m_color, { 1.0f, 0.0f, 0.0f}},

        {{-1.0f, -1.0f,  1.0f}, m_color, {-1.0f, 0.0f, 0.0f}}, // x-
        {{-1.0f,  1.0f,  1.0f}, m_color, {-1.0f, 0.0f, 0.0f}},
        {{-1.0f,  1.0f, -1.0f}, m_color, {-1.0f, 0.0f, 0.0f}},
        {{-1.0f, -1.0f, -1.0f}, m_color, {-1.0f, 0.0f, 0.0f}},

        {{ 1.0f,  1.0f,  1.0f}, m_color, {0.0f,  1.0f, 0.0f}}, // y+
        {{ 1.0f,  1.0f, -1.0f}, m_color, {0.0f,  1.0f, 0.0f}},
        {{-1.0f,  1.0f, -1.0f}, m_color, {0.0f,  1.0f, 0.0f}},
        {{-1.0f,  1.0f,  1.0f}, m_color, {0.0f,  1.0f, 0.0f}},

        {{ 1.0f, -1.0f,  1.0f}, m_color, {0.0f, -1.0f, 0.0f}}, // y-
        {{-1.0f, -1.0f,  1.0f}, m_color, {0.0f, -1.0f, 0.0f}},
        {{-1.0f, -1.0f, -1.0f}, m_color, {0.0f, -1.0f, 0.0f}},
        {{ 1.0f, -1.0f, -1.0f}, m_color, {0.0f, -1.0f, 0.0f}},

        {{-1.0f, -1.0f,  1.0f}, m_color, {0.0f, 0.0f,  1.0f}}, // z+
        {{ 1.0f, -1.0f,  1.0f}, m_color, {0.0f, 0.0f,  1.0f}},
        {{ 1.0f,  1.0f,  1.0f}, m_color, {0.0f, 0.0f,  1.0f}},
        {{-1.0f,  1.0f,  1.0f}, m_color, {0.0f, 0.0f,  1.0f}},

        {{-1.0f, -1.0f, -1.0f}, m_color, {0.0f, 0.0f, -1.0f}}, // z-
        {{-1.0f,  1.0f, -1.0f}, m_color, {0.0f, 0.0f, -1.0f}},
        {{ 1.0f,  1.0f, -1.0f}, m_color, {0.0f, 0.0f, -1.0f}},
        {{ 1.0f, -1.0f, -1.0f}, m_color, {0.0f, 0.0f, -1.0f}},
    };

    std::vector<uint32_t> indices = 
    {
         0, 1, 2, 2, 3, 0,
         4, 5, 6, 6, 7, 4,
         8, 9,10,10,11, 8,
        12,13,14,14,15,12,
        16,17,18,18,19,16,
        20,21,22,22,23,20,
    };

    shapes.push_back({vertices, indices, getTransform()});
}