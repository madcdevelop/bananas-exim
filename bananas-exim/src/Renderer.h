#pragma once

#include "Camera.h"
#include "Mesh.h"
#include "Model.h"
#include "Shader.h"
#include "Window.h"

namespace Core
{

class Renderer {

public:
    Mesh* m_Mesh;
    Camera m_Camera;

private:
    Window* m_Window;
    Shader m_Shader1;
    Shader m_ShaderLight;

public:
    Renderer(Window* window, Mesh* mesh);
    ~Renderer();

    void Init();
    void Draw(float timestep);

};

}