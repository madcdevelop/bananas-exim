#ifndef GRAPHICS_ENGINE_PLATFORM_WIN32_H
#define GRAPHICS_ENGINE_PLATFORM_WIN32_H

#include "../Core/Common.h"
#include "../Core/Timestep.h"
#include "../Graphics/Renderer.h"

LRESULT CALLBACK WndProcEngine(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace PlatformEngine
{

class PlatformWin32
{

public:
    HWND            m_hWnd;
    HINSTANCE       m_hInstance;
    HDC             m_hDeviceContext;
    HGLRC           m_hRenderContext;
    UINT            m_width;
    UINT            m_height;
    DWORD           m_windowStyle;
    const wchar_t*  m_windowTitle;

    GraphicsEngine::Renderer*               m_renderer;
    GraphicsEngine::RenderDeviceOpenGL*     m_renderDevice;
    CoreEngine::Timestep*   m_timestep;

private:
    // Camera Movement
    bool  m_firstMouse = false;
    real32 m_lastX = 0.0f;
    real32 m_lastY = 0.0f;

    // Timestep
    real32 m_deltaTime;
    
public:
    PlatformWin32(HINSTANCE hInstance, HWND hwnd);
    ~PlatformWin32();

    bool Win32CreateWindow();
    bool Init();

    int32 Run();
    LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    void CameraKeyboardCallback();

    static std::string GetFilePath(const wchar_t* relativePath);

private:
    void CameraMouseCallback(const POINT& pos);
};

}

#endif
