#ifndef GRAPHICS_ENGINE_PLATFORM_WIN32_H
#define GRAPHICS_ENGINE_PLATFORM_WIN32_H

#include "../Core/Common.h"
#include "../Core/Timestep.h"
#include "../Graphics/Renderer.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace PlatformEngine
{

class PlatformWin32
{

public:
    HWND            m_hWnd;
    HINSTANCE       m_hInstance;
    HDC             m_hDeviceContext;
    HGLRC           m_hRenderContext;
    UINT            m_Width;
    UINT            m_Height;
    DWORD           m_WindowStyle;
    const wchar_t*  m_WindowTitle;

    GraphicsEngine::Renderer*               m_Renderer;
    GraphicsEngine::RenderDeviceOpenGL*     m_RenderDevice;
    CoreEngine::Timestep*   m_Timestep;

private:
    // Camera Movement
    bool  m_FirstMouse = false;
    real32 m_LastX = 0.0f;
    real32 m_LastY = 0.0f;

    // Timestep
    real32 m_DeltaTime;
    
public:
    PlatformWin32(HINSTANCE hInstance, HWND hwnd);
    ~PlatformWin32();

    bool Win32CreateWindow();
    bool Init();

    int32 Run();
    LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    void CameraKeyboardCallback();

private:
    void CameraMouseCallback(const POINT& pos);
};

}

#endif