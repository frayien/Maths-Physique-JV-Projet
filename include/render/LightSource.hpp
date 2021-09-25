#ifndef MPJVP_LIGHTSOURCE
#define MPJVP_LIGHTSOURCE

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

class LightSource
{
private:
    glm::vec3 m_position = {0.0f, 0.0f, 0.0f};
    glm::vec3 m_color = {1.0f, 1.0f, 1.0f};
    float m_ambient = 0.1f;
    
public:
    LightSource() {}
    ~LightSource() {}

    inline void translate(glm::vec3 dir) { m_position += dir; }

    inline glm::vec3 getPosition() const { return m_position; }
    inline glm::vec3 getColor() const { return m_color; }
    inline float getAmbient() const { return m_ambient; }

    inline void setPosition(glm::vec3 pos) { m_position = pos; }
    inline void setColor(glm::vec3 color) { m_color = color; }
    inline void setAmbient(float ambient) { m_ambient = ambient; }
};

#endif // MPJVP_LIGHTSOURCE
