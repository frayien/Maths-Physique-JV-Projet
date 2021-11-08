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
    std::array<float, 12> mat34Values = rotationMat34.getValues();

    const glm::mat4 rotation =
    {
        mat34Values[0], mat34Values[1], mat34Values[2], 0.0f,
        mat34Values[4], mat34Values[5], mat34Values[6], 0.0f,
        mat34Values[8], mat34Values[9], mat34Values[10], 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f,
    };

    m_cube.setPosition(rigidbodyPos);
    m_cube.setRotation(rotation);
    m_cube.setScale(m_scale);
    m_cube.addShape(shapes);
}