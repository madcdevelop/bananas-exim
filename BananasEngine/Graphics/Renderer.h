#ifndef GRAPHICS_ENGINE_RENDERER_H
#define GRAPHICS_ENGINE_RENDERER_H

#include "Scene.h"
#include "Window.h"

namespace GraphicsEngine
{

class Renderer {

public:
    Scene* m_Scene;

private:
    Window* m_Window;

public:
    Renderer(Window* window);
    ~Renderer();

    void Init();
    void Draw(float timestep);

};

}

#endif