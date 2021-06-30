#pragma once

#include "Common.h"
#include "Camera.h"
#include "Model.h"
#include "Shader.h"
#include "Window.h"

namespace Core
{

class Scene
{
public:
    std::vector<Model> m_Models;
    Camera m_Camera;

private:
    Shader m_Shader1;
    Shader m_ShaderLight;

public:
    Scene();
    ~Scene();

    void LoadModels();
    void Draw(float timestep, Window* window);

};

}
