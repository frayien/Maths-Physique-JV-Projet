#ifndef MPJVP_CUBESHAPEGENERATOR
#define MPJVP_CUBESHAPEGENERATOR

#include "render/shape/IShapeGenerator.hpp"
#include "render/shape/Transformable.hpp"

class CubeShapeGenerator : public IShapeGenerator, public Transformable
{
public:
    CubeShapeGenerator(glm::vec3 color = { 0.5f, 0.5f, 0.5f });
    ~CubeShapeGenerator();

    virtual void addShape(std::vector<Shape> & shapes) const override;

private:
    glm::vec3 m_color;
};

#endif // MPJVP_CUBESHAPEGENERATOR
