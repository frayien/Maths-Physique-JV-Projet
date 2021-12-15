#include "ContactShapeGenerator.hpp"

ContactShapeGenerator::ContactShapeGenerator(RigidBodyContact contact, color_t color) :
    m_contact{contact},
    m_cube{color},
    m_sphere{color}
{

}

void ContactShapeGenerator::addShape(std::vector<Shape> & shapes)
{
    // Sphere
    m_sphere.setPosition(m_contact.getPosition());
    m_sphere.setScale(m_scale);
    m_sphere.addShape(shapes);

    // Cube
    const Vector3f defaultDir {0.f, 0.f, 1.f};
    const Vector3f dir = m_contact.getNormal().normalize();
    const float angle = -glm::acos(dir.dotProduct(defaultDir));

    m_cube.setPosition(m_contact.getPosition() + m_contact.getNormal().normalize() * 2.0f * m_scale);
    m_cube.resetRotation();
    m_cube.rotate(angle, dir.crossProduct(defaultDir));
    m_cube.setScale({m_scale / 2.0f, m_scale/ 2.0f, 2.0f * m_scale});
    m_cube.addShape(shapes);
}