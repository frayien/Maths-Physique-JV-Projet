#ifndef MPJVP_TRANSFORMABLE
#define MPJVP_TRANSFORMABLE

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transformable
{
private:
    glm::vec3 m_position = {0.f, 0.f, 0.f};
    glm::mat4 m_rotation = glm::mat4{1.f};
    glm::vec3 m_scale = {1.f, 1.f, 1.f};
    
public:
    Transformable();
    virtual ~Transformable();

    inline void setPosition(const glm::vec3 & vect) { m_position = vect; }
    inline void translate(const glm::vec3 & vect) { m_position += vect; }
    void rotate(float angle, const glm::vec3 & axis);
    inline void resetRotation() { m_rotation = glm::mat4(1.0f); }
    void scale(const glm::vec3 & vect);
    void scale(float factor);
    void setScale(const glm::vec3 & vect);
    void setScale(float factor);

    glm::mat4 getTransform() const;
};

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


#endif // MPJVP_TRANSFORMABLE
