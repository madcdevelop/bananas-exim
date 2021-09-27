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
    std::string m_Name = "camera";
    glm::vec3 m_Position;
    glm::vec3 m_Front;
    glm::vec3 m_Up;
    glm::vec3 m_Right;

    real32 m_Yaw;
    real32 m_Pitch;
    real32 m_Fov;
    real32 m_MovementSpeed;
    real32 m_Sensitivity;

public:
    Camera(glm::vec3 position, real32 yaw, real32 pitch);
    ~Camera();

    void KeyboardMovement(CameraMovement direction, real32 deltaTime);
    void MouseMovement(real32 xoffset, real32 yoffset);

private:
    void UpdateCameraVectors();
};
  
}

#endif