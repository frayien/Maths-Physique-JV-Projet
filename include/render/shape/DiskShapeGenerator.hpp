#ifndef MPJVP_DISKSHAPEGENERATOR
#define MPJVP_DISKSHAPEGENERATOR

#include "render/shape/IShapeGenerator.hpp"
#include "render/shape/Transformable.hpp"

class DiskShapeGenerator : public IShapeGenerator, public Transformable
{
public:
    DiskShapeGenerator(glm::vec3 color = { 0.5f, 0.5f, 0.5f });
    ~DiskShapeGenerator();

    virtual void addShape(std::vector<Shape> & shapes) const override;

private:
    glm::vec3 m_color;
};

#endif // MPJVP_DISKSHAPEGENERATOR
