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
    inline void setRotation(const glm::mat4 & mat) { m_rotation = mat; }
    inline void translate(const glm::vec3 & vect) { m_position += vect; }
    void rotate(float angle, const glm::vec3 & axis);
    inline void resetRotation() { m_rotation = glm::mat4(1.0f); }
    void scale(const glm::vec3 & vect);
    void scale(float factor);
    void setScale(const glm::vec3 & vect);
    void setScale(float factor);

    glm::mat4 getTransform() const;
};

#endif // MPJVP_TRANSFORMABLE
