#ifndef GRAPHICS_ENGINE_RENDERER_H
#define GRAPHICS_ENGINE_RENDERER_H

// TODO(neil): replace with interface RenderDevice
#include "RenderDeviceOpenGL.h"

namespace GraphicsEngine
{

class Renderer {

public:
    RenderDeviceOpenGL* m_Device;
    HINSTANCE m_hInstance;

public:
    Renderer(HINSTANCE hInstance);
    ~Renderer();

    void CreateRenderDevice(RenderDeviceOpenGL* renderDevice);
    void Draw(float timestep);

};

}

#endif