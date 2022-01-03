#ifndef GRAPHICS_ENGINE_RENDER_DEVICE_OPEN_GL_H
#define GRAPHICS_ENGINE_RENDER_DEVICE_OPEN_GL_H

#include "../Core/Common.h"
#include "Scene.h"

#include <thread>

// Flag to change between using Main thread vs creating new thread 
#define USE_THREAD_TO_LOAD_OPENGL_MESH_TEXTURES_FEATURE_FLAG 0

namespace GraphicsEngine
{


class RenderDeviceOpenGL
{
public:
    HWND    m_hWnd;
    HDC     m_hDeviceContext;
    HGLRC   m_hRenderContext;
    UINT    m_width;
    UINT    m_height;
    bool    m_running;

    Scene*  m_scene;
    std::thread m_loadDataModelThread;
    int32 m_meshTexturesLoaded = 0;

public:
    RenderDeviceOpenGL();
    ~RenderDeviceOpenGL();

    void Render();

    int32 InitMeshesTextures();
    bool Init();
    void Shutdown();
    void Resize();
};

}

#endif