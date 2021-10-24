#ifndef MPJVP_SPHERESHAPEGENERATOR
#define MPJVP_SPHERESHAPEGENERATOR

#include "render/shape/IShapeGenerator.hpp"
#include "render/shape/Transformable.hpp"

#include "render/Color.hpp"

class SphereShapeGenerator : public IShapeGenerator, public Transformable
{
public:
    SphereShapeGenerator(color_t color = Color::GRAY);
    ~SphereShapeGenerator();

    virtual void addShape(std::vector<Shape> & shapes) override;

private:
    void constructVertices();

    color_t m_color;
    float m_radius = 1.0f;

    std::vector<Vertex> m_vertices;
    std::vector<uint32_t> m_indices;
};

#endif // MPJVP_SPHERESHAPEGENERATOR
