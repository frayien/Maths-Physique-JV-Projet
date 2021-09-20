#include "render/Camera.hpp"

Camera::Camera() :
    m_position{0.0f, 0.0f, 0.0f},
    m_yaw{0.0f},
    m_pitch{0.0f}
{
}

Camera::~Camera()
{
}

void Camera::rotateYaw(float yaw)
{
    m_yaw += yaw;
    
}
void Camera::rotatePitch(float pitch)
{
    m_pitch += pitch;
    m_pitch = std::clamp(m_pitch, -glm::pi<float>()/2.0f + 0.01f, glm::pi<float>()/2.0f - 0.01f);
}

void Camera::move(glm::vec3 dir)
{
    m_position += glm::rotate(dir, m_yaw, glm::vec3(0.0f, 0.0f, 1.0f));
}

glm::vec3 Camera::getDirection()
{ 
    glm::vec3 direction{1.0f, 0.0f, 0.0f};
    direction = glm::rotate(direction, m_yaw, glm::vec3(0.0f, 0.0f, 1.0f));
    direction = glm::rotate(direction, m_pitch, glm::cross(direction, glm::vec3(0.0f, 0.0f, 1.0f)));

    return direction; 
}