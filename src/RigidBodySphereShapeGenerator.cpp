#include "RigidBodySphereShapeGenerator.hpp"

RigidBodySphereShapeGenerator::RigidBodySphereShapeGenerator(RigidBody* rigidbody, color_t color) :
    m_rigidbody{rigidbody},
    m_sphere{color}
{

}

void RigidBodySphereShapeGenerator::addShape(std::vector<Shape> & shapes)
{
    m_sphere.setPosition(m_rigidbody->getPosition());
    m_sphere.setScale(m_scale);
    m_sphere.addShape(shapes);
}