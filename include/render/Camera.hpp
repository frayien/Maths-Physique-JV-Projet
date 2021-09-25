#ifndef MPJVP_CAMERA
#define MPJVP_CAMERA

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <algorithm>

class Camera
{
private:
    glm::vec3 m_position;
    float m_yaw;
    float m_pitch;

    float m_viewDistance;

public:
    Camera();
    virtual ~Camera();

    void rotateYaw(float yaw);
    void rotatePitch(float pitch);

    void move(glm::vec3 dir);
    void translate(glm::vec3 vect);

    inline glm::vec3 getPosition() const { return m_position; }
    glm::vec3 getDirection() const;
    inline float getViewDistance() const { return m_viewDistance; }

    inline void setPosition(glm::vec3 pos) { m_position = pos; }
    inline void setRotation(float yaw, float pitch) { m_yaw = yaw; m_pitch = pitch; }
    inline void setViewDistance(float viewDistance) { m_viewDistance = viewDistance; }
};

#endif // MPJVP_CAMERA
