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
    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_up;
    glm::vec3 m_right;

    real32 m_yaw;
    real32 m_pitch;
    real32 m_fov;
    real32 m_movementSpeed;
    real32 m_sensitivity;

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