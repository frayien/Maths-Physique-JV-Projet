#ifndef MPJVP_DISKSHAPEGENERATOR
#define MPJVP_DISKSHAPEGENERATOR

#include "render/shape/IShapeGenerator.hpp"
#include "render/shape/Transformable.hpp"

class DiskShapeGenrator : public IShapeGenerator, public Transformable
{
public:
    DiskShapeGenrator(glm::vec3 color = { 0.5f, 0.5f, 0.5f });
    ~DiskShapeGenrator();

    virtual void addShape(std::vector<Shape> & shapes) const override;

private:
    glm::vec3 m_color;
};

DiskShapeGenrator::DiskShapeGenrator(glm::vec3 color) :
    m_color{color}
{
}

DiskShapeGenrator::~DiskShapeGenrator()
{
}

void DiskShapeGenrator::addShape(std::vector<Shape> & shapes) const
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

#endif // MPJVP_DISKSHAPEGENERATOR
