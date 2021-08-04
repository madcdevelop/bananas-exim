#include "Camera.h"

namespace GraphicsEngine 
{

Camera::Camera(glm::vec3 position, real32 yaw, real32 pitch)
    : m_Position(position), m_Front(glm::vec3(0.0f, 0.0f, -1.0f)),
      m_Up(glm::vec3(0.0f, 1.0f, 0.0f)), m_Yaw(yaw), m_Pitch(pitch),
      m_Fov(45.0f), m_MovementSpeed(0.02f), m_Sensitivity(0.1f)
{
    UpdateCameraVectors();
}

Camera::~Camera()
{
}

void Camera::KeyboardMovement(CameraMovement direction, real32 deltaTime)
{
    real32 velocity = m_MovementSpeed * deltaTime;
    if(direction == FORWARD)
        m_Position += velocity * m_Front;
    if(direction == BACKWARD)
        m_Position -= velocity * m_Front;
    if(direction == LEFT)
        m_Position -= velocity * m_Right;
    if(direction == RIGHT)
        m_Position += velocity * m_Right;
}

void Camera::MouseMovement(real32 xoffset, real32 yoffset)
{
    xoffset *= m_Sensitivity;
    yoffset *= m_Sensitivity;

    m_Yaw   += xoffset;
    m_Pitch += yoffset;

    // if pitch is out of bounds, screen doesn't get flipped
    if(m_Pitch > 89.0f)
        m_Pitch = 89.0f;
    if(m_Pitch < -89.0f)
        m_Pitch = -89.0f;

    UpdateCameraVectors();
}

void Camera::UpdateCameraVectors()
{
    glm::vec3 direction;
    direction.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    direction.y = sin(glm::radians(m_Pitch));
    direction.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));

    m_Front = glm::normalize(direction);
    m_Right = glm::normalize(glm::cross(m_Front, m_Up));
}


}