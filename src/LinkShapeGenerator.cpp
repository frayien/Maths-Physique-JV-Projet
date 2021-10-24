#include "LinkShapeGenerator.hpp"

LinkShapeGenerator::LinkShapeGenerator(Particle* particleA, Particle* particleB, color_t color) :
    m_particleA{particleA},
    m_particleB{particleB},
    m_cube{color}
{
}

LinkShapeGenerator::~LinkShapeGenerator()
{
}

void LinkShapeGenerator::addShape(std::vector<Shape> & shapes)
{
    const Vector3f posA = m_particleA->getPosition();
    const Vector3f posB = m_particleB->getPosition();
    const Vector3f pos = (posA + posB)/2.0f;

    const Vector3f defaultDir {0.f, 0.f, 1.f};
    const Vector3f dir = (posA - posB).normalize();

    const float angle = -glm::acos(dir.dotProduct(defaultDir));
    const float len = (posA - posB).norm();

    m_cube.setPosition(pos);
    m_cube.resetRotation();
    m_cube.rotate(angle, dir.crossProduct(defaultDir));
    m_cube.setScale({0.05f, 0.05f, len / 2.f});
    m_cube.addShape(shapes);
}