#include "Camera.h"

namespace GraphicsEngine 
{

Camera::Camera(glm::vec3 position, real32 yaw, real32 pitch)
    : m_position(position), m_front(glm::vec3(0.0f, 0.0f, -1.0f)),
      m_up(glm::vec3(0.0f, 1.0f, 0.0f)), m_yaw(yaw), m_pitch(pitch),
      m_fov(45.0f), m_movementSpeed(0.02f), m_sensitivity(0.1f)
{
    UpdateCameraVectors();
}

Camera::~Camera()
{
}

void Camera::KeyboardMovement(CameraMovement direction, real32 deltaTime)
{
    real32 velocity = m_movementSpeed * deltaTime;
    if(direction == FORWARD)
        m_position += velocity * m_front;
    if(direction == BACKWARD)
        m_position -= velocity * m_front;
    if(direction == LEFT)
        m_position -= velocity * m_right;
    if(direction == RIGHT)
        m_position += velocity * m_right;
}

void Camera::MouseMovement(real32 xoffset, real32 yoffset)
{
    xoffset *= m_sensitivity;
    yoffset *= m_sensitivity;

    m_yaw   += xoffset;
    m_pitch += yoffset;

    // if pitch is out of bounds, screen doesn't get flipped
    if(m_pitch > 89.0f)
        m_pitch = 89.0f;
    if(m_pitch < -89.0f)
        m_pitch = -89.0f;

    UpdateCameraVectors();
}

void Camera::UpdateCameraVectors()
{
    glm::vec3 direction;
    direction.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    direction.y = sin(glm::radians(m_pitch));
    direction.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

    m_front = glm::normalize(direction);
    m_right = glm::normalize(glm::cross(m_front, m_up));
}


}