#ifndef MPJVP_HUMANSHAPEGENERATOR
#define MPJVP_HUMANSHAPEGENERATOR

#include "render/shape/IShapeGenerator.hpp"
#include "render/shape/CubeShapeGenerator.hpp"
#include "render/shape/SphereShapeGenerator.hpp"

#include "physics/RigidBody.hpp"

class HumanShapeGenerator : public IShapeGenerator
{
private:
    RigidBody * m_rigidbody;
    SphereShapeGenerator m_head;
    CubeShapeGenerator m_chest;
    CubeShapeGenerator m_leftArm;
    CubeShapeGenerator m_rightArm;
    CubeShapeGenerator m_leftLeg;
    CubeShapeGenerator m_rightLeg;

    glm::vec3 m_scale = {0.2f, 2.0f, 2.0f};
    
public:
    HumanShapeGenerator(RigidBody* rigidbody, color_t color = Color::GRAY);
    virtual ~HumanShapeGenerator();

    virtual void addShape(std::vector<Shape> & shapes) override;

    inline void setScale(float scale) { m_scale = {0.2f * scale, 2.0f * scale, 2.0f * scale}; }
    inline void setScale(glm::vec3 scale) { m_scale = scale; }
};

#endif // MPJVP_HUMANSHAPEGENERATOR