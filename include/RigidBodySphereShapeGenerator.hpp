#ifndef MPJVP_RIGIDBODYSPHERESHAPEGENERATOR
#define MPJVP_RIGIDBODYSPHERESHAPEGENERATOR

#include "render/shape/IShapeGenerator.hpp"
#include "render/shape/SphereShapeGenerator.hpp"

#include "physics/RigidBody.hpp"

class RigidBodySphereShapeGenerator : public IShapeGenerator
{
private:
    RigidBody * m_rigidbody;
    SphereShapeGenerator m_sphere;

    float m_scale = 0.5f;
    
public:
    RigidBodySphereShapeGenerator(RigidBody* rigidbody, color_t color = Color::GRAY);
    virtual ~RigidBodySphereShapeGenerator() = default;

    virtual void addShape(std::vector<Shape> & shapes) override;

    inline void setScale(float scale) { m_scale = scale; }
};

#endif // MPJVP_RIGIDBODYSPHERESHAPEGENERATOR