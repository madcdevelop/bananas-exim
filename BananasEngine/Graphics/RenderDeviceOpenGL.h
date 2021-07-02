#ifndef GRAPHICS_ENGINE_RENDER_DEVICE_OPEN_GL_H
#define GRAPHICS_ENGINE_RENDER_DEVICE_OPEN_GL_H

#include "../Core/Common.h"

namespace GraphicsEngine
{

class RenderDeviceOpenGL
{
public:
    HWND    m_hWnd;
    HDC     m_hDeviceContext;
    HGLRC   m_hRenderContext;
    UINT    m_Width;
    UINT    m_Height;
    bool    m_Running;

public:
    RenderDeviceOpenGL();
    ~RenderDeviceOpenGL();


    void Render();
    bool Init();
    void Shutdown();
    void Resize();
};

}

#endif