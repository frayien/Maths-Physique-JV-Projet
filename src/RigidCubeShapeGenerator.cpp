#include "RigidCubeShapeGenerator.hpp"
#include "physics/Quaternion.hpp"
#include "physics/Vector3f.hpp"

RigidCubeShapeGenerator::RigidCubeShapeGenerator(RigidBody * rigidbody, color_t color) :
    m_rigidbody{rigidbody},
    m_cube{color}
{

}

RigidCubeShapeGenerator::~RigidCubeShapeGenerator()
{

}

void RigidCubeShapeGenerator::addShape(std::vector<Shape> & shapes)
{
    const Vector3f rigidbodyPos = m_rigidbody->getPosition();
    const Quaternion rigidbodyQuaternion = m_rigidbody->getQuaternion();

    const float quaternionRotationAngle = glm::acos(rigidbodyQuaternion.getW()) * 2.0f;
    float sinAngle = glm::sin(quaternionRotationAngle / 2.0);

    const Vector3f quaternionRotationAxis
    {
        rigidbodyQuaternion.getX() / sinAngle,
        rigidbodyQuaternion.getY() / sinAngle,
        rigidbodyQuaternion.getZ() / sinAngle
    };

    m_cube.setPosition(rigidbodyPos);
    m_cube.resetRotation();
    m_cube.rotate(quaternionRotationAngle, quaternionRotationAxis);
    m_cube.setScale(m_scale);
    m_cube.addShape(shapes);
}