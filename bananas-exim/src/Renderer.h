#pragma once

#include "Camera.h"
#include "Model.h"
#include "Shader.h"
#include "Window.h"

namespace Core
{

class Renderer {

public:
    Model* m_Model;
    Camera m_Camera;

private:
    Window* m_Window;
    Shader m_Shader1;
    Shader m_ShaderLight;

public:
    Renderer(Window* window, Model* model);
    ~Renderer();

    void Init();
    void Draw(float timestep);

};

}