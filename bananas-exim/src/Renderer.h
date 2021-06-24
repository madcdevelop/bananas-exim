#pragma once

#include "Camera.h"
#include "Scene.h"
#include "Shader.h"
#include "Window.h"

namespace Core
{

class Renderer {

public:
    Scene* m_Scene;
    Camera m_Camera;

private:
    Window* m_Window;
    Shader m_Shader1;
    Shader m_ShaderLight;

public:
    Renderer(Window* window);
    ~Renderer();

    void Init();
    void Draw(float timestep);

};

}