#pragma once

#include "Common.h"

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

private:
    double m_LastFrameTime = 0.0;

public:
    Window(HINSTANCE hInstance, HWND hwnd);
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