#ifndef GRAPHICS_ENGINE_SCENE_H
#define GRAPHICS_ENGINE_SCENE_H

#include "../Core/Common.h"
#include "Camera.h"
#include "Model.h"
#include "Shader.h"
#include "Window.h"

namespace GraphicsEngine
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

#endif