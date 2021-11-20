#include "RigidBodyLinkShapeGenerator.hpp"

#include <iostream>

RigidBodyLinkShapeGenerator::RigidBodyLinkShapeGenerator(RigidBody* rigidbodyA, Vector3f bodyPointA, RigidBody* rigidbodyB, Vector3f bodyPointB, color_t color) :
    m_rigidbodyA{rigidbodyA},
    m_bodyPointA{bodyPointA},
    m_rigidbodyB{rigidbodyB},
    m_bodyPointB{bodyPointB},
    m_cube{color}
{

}

RigidBodyLinkShapeGenerator::~RigidBodyLinkShapeGenerator()
{

}

void RigidBodyLinkShapeGenerator::addShape(std::vector<Shape> & shapes)
{
    const Vector3f posA = m_rigidbodyA->getTranformMatrix() * m_bodyPointA;
    const Vector3f posB = m_rigidbodyB->getTranformMatrix() * m_bodyPointB;

    static int count = 0;
    count++;
    if (count > 10)
    {
        std::cout << "=============================" << std::endl;
        std::cout << "Quaternion 1 : " << std::endl << m_rigidbodyA->getQuaternion() << std::endl;
        std::cout << "Pos A : " << posA << std::endl;
        std::cout << "=============================" << std::endl;
        count = 0;
    }

    const Vector3f pos = (posA + posB) / 2.0f;

    const Vector3f defaultDir {0.f, 0.f, 1.f};
    const Vector3f dir = (posA - posB).normalize();

    const float angle = -glm::acos(dir.dotProduct(defaultDir));
    const float len = (posA - posB).norm();

    m_cube.setPosition(pos);
    m_cube.resetRotation();
    m_cube.rotate(angle, dir.crossProduct(defaultDir));
    m_cube.setScale({m_width, m_width, len / 2.f});
    m_cube.addShape(shapes);
}

// LinkShapeGenerator::LinkShapeGenerator(Particle* particleA, Particle* particleB, color_t color) :
//     m_particleA{particleA},
//     m_particleB{particleB},
//     m_cube{color}
// {
// }

// LinkShapeGenerator::~LinkShapeGenerator()
// {
// }

// void LinkShapeGenerator::addShape(std::vector<Shape> & shapes)
// {
//     const Vector3f posA = m_particleA->getPosition();
//     const Vector3f posB = m_particleB->getPosition();
//     const Vector3f pos = (posA + posB)/2.0f;

//     const Vector3f defaultDir {0.f, 0.f, 1.f};
//     const Vector3f dir = (posA - posB).normalize();

//     const float angle = -glm::acos(dir.dotProduct(defaultDir));
//     const float len = (posA - posB).norm();

//     m_cube.setPosition(pos);
//     m_cube.resetRotation();
//     m_cube.rotate(angle, dir.crossProduct(defaultDir));
//     m_cube.setScale({m_width, m_width, len / 2.f});
//     m_cube.addShape(shapes);
// }