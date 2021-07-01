#ifndef GRAPHICS_ENGINE_CAMERA_H
#define GRAPHICS_ENGINE_CAMERA_H

#include "../Core/Common.h"

namespace GraphicsEngine 
{

enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera {

public:
    glm::vec3 m_Position;
    glm::vec3 m_Front;
    glm::vec3 m_Up;
    glm::vec3 m_Right;


private:
    float m_Yaw;
    float m_Pitch;
    float m_Fov;
    float m_MovementSpeed;
    float m_Sensitivity;

public:
    Camera(glm::vec3 position, float yaw, float pitch);
    ~Camera();

    void KeyboardMovement(CameraMovement direction, float deltaTime);
    void MouseMovement(float xoffset, float yoffset);

private:
    void UpdateCameraVectors();
};
  
}

#endif