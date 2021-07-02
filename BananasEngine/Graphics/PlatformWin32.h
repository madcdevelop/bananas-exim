#ifndef GRAPHICS_ENGINE_PLATFORM_WIN32_H
#define GRAPHICS_ENGINE_PLATFORM_WIN32_H

#include "../Core/Common.h"

namespace GraphicsEngine
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

public:
    PlatformWin32(HINSTANCE hInstance, HWND hwnd);
    ~PlatformWin32();

    bool Win32CreateWindow();
    bool Init();

};

}

#endif