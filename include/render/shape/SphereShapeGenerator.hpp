#ifndef MPJVP_SPHERESHAPEGENERATOR
#define MPJVP_SPHERESHAPEGENERATOR

#include "render/shape/IShapeGenerator.hpp"
#include "render/shape/Transformable.hpp"

class SphereShapeGenerator : public IShapeGenerator, public Transformable
{
public:
    SphereShapeGenerator(glm::vec3 color = { 0.5f, 0.5f, 0.5f });
    ~SphereShapeGenerator();

    virtual void addShape(std::vector<Shape> & shapes) const override;

private:
    void constructVertices();

    glm::vec3 m_color;
    float m_radius = 1.0f;

    std::vector<Vertex> m_vertices;
    std::vector<uint32_t> m_indices;
};

#endif // MPJVP_SPHERESHAPEGENERATOR
