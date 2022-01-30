#ifndef GRAPHICS_ENGINE_RENDERER_H
#define GRAPHICS_ENGINE_RENDERER_H

#include "RenderDeviceOpenGL.h"

namespace GraphicsEngine
{

class Renderer {

public:
    RenderDeviceOpenGL* m_device;
    HINSTANCE m_hInstance;

public:
    Renderer(HINSTANCE hInstance);
    ~Renderer();

    void CreateRenderDevice(RenderDeviceOpenGL* renderDevice);
    void Draw(real32 timestep);

};

}

#endif
