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
    UINT    m_Width;
    UINT    m_Height;
    bool    m_Running;

    Scene*  m_Scene;
    std::thread m_LoadDataModelThread;
    int32 m_MeshTexturesLoaded = 0;

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