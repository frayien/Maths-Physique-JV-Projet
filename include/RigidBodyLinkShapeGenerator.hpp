#ifndef MPJVP_RIGIDBODYLINKSHAPEGENERATOR
#define MPJVP_RIGIDBODYLINKSHAPEGENERATOR

#include "render/shape/IShapeGenerator.hpp"

#include "render/shape/CubeShapeGenerator.hpp"
#include "physics/RigidBody.hpp"

class RigidBodyLinkShapeGenerator : public IShapeGenerator
{
private:
    RigidBody* m_rigidbodyA;
    Vector3f m_bodyPointA;
    RigidBody* m_rigidbodyB;
    Vector3f m_bodyPointB;
    CubeShapeGenerator m_cube;

    float m_width = 0.05f;
    
public:
    RigidBodyLinkShapeGenerator(RigidBody* rigidbodyA, Vector3f bodyPointA, RigidBody* rigidbodyB, Vector3f bodyPointB, color_t color = Color::GRAY);
    virtual ~RigidBodyLinkShapeGenerator();

    virtual void addShape(std::vector<Shape> & shapes) override;

    inline void setWidth(float width) { m_width = width; }
};


#endif // MPJVP_RIGIDBODYLINKSHAPEGENERATOR
