#include "render/shape/Transformable.hpp"

Transformable::Transformable()
{
}

Transformable::~Transformable()
{
}

void Transformable::rotate(float angle, const glm::vec3 & axis)
{
    m_rotation = glm::rotate(m_rotation, angle, glm::normalize(axis));
}

void Transformable::scale(const glm::vec3 & vect)
{
    m_scale.x *= vect.x;
    m_scale.y *= vect.y;
    m_scale.z *= vect.z;
}

void Transformable::scale(float factor)
{
    m_scale *= factor;
}

void Transformable::setScale(const glm::vec3 & vect)
{
    m_scale = vect;
}

void Transformable::setScale(float factor)
{
    m_scale = glm::vec3(1.f) * factor;
}

glm::mat4 Transformable::getTransform() const
{
    glm::mat4 transform{1.0f};
    transform = glm::translate(transform, m_position);
    transform *= m_rotation;
    transform = glm::scale(transform, m_scale);
    return transform;
}
