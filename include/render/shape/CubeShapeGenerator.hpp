#ifndef MPJVP_CUBESHAPEGENERATOR
#define MPJVP_CUBESHAPEGENERATOR

#include "render/shape/IShapeGenerator.hpp"
#include "render/shape/Transformable.hpp"

class CubeShapeGenrator : public IShapeGenerator, public Transformable
{
public:
    CubeShapeGenrator(glm::vec3 color = { 0.5f, 0.5f, 0.5f });
    ~CubeShapeGenrator();

    virtual void addShape(std::vector<Shape> & shapes) const override;

private:
    glm::vec3 m_color;
};

CubeShapeGenrator::CubeShapeGenrator(glm::vec3 color) :
    m_color{color}
{
}

CubeShapeGenrator::~CubeShapeGenrator()
{
}

void CubeShapeGenrator::addShape(std::vector<Shape> & shapes) const
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


#endif // MPJVP_CUBESHAPEGENERATOR
