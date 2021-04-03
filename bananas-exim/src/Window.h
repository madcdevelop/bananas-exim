#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <glad\glad.h>

namespace Core
{

class Window {

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
    Window();
    Window(HINSTANCE hInstance);
    ~Window();

    // Main Window Loop
    int Run();

    // Framework methods
    bool Init();
    LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

protected:
    bool InitWindow();
    bool InitGL();
    void Render();
    void Shutdown();
};

}