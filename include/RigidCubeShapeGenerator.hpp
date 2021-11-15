#ifndef MPJVP_RIGIDCUBESHAPEGENERATOR
#define MPJVP_RIGIDCUBESHAPEGENERATOR

#include "render/shape/IShapeGenerator.hpp"
#include "render/shape/CubeShapeGenerator.hpp"

#include "physics/RigidBody.hpp"

class RigidCubeShapeGenerator : public IShapeGenerator
{
private:
    RigidBody * m_rigidbody;
    CubeShapeGenerator m_cube;

    glm::vec3 m_scale = {0.5f, 0.5f, 0.5f};
    
public:
    RigidCubeShapeGenerator(RigidBody* m_rigidbody, color_t color = Color::GRAY);
    virtual ~RigidCubeShapeGenerator();

    virtual void addShape(std::vector<Shape> & shapes) override;

    inline void setScale(float scale) { m_scale = {scale, scale, scale}; }
    inline void setScale(glm::vec3 scale) { m_scale = scale; }
};

#endif // MPJVP_RIGIDCUBESHAPEGENERATOR