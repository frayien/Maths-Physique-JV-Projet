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
    // Position
    const Vector3f rigidbodyPos = m_rigidbody->getPosition();

    // Rotation
    Matrix34 rotationMat34;
    rotationMat34.setOrientationAndPosition(m_rigidbody->getQuaternion(), Vector3f{0.0f, 0.0f, 0.0f});

    const glm::mat4 rotation = rotationMat34.extractMatrix33();

    m_cube.setPosition(rigidbodyPos);
    m_cube.setRotation(rotation);
    m_cube.setScale(m_scale);
    m_cube.addShape(shapes);
}