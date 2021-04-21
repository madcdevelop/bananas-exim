#pragma once

#include "Common.h"

// Virtual Key Codes
// https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
#define BANANAS_KEY_W 0x57
#define BANANAS_KEY_S 0x53
#define BANANAS_KEY_D 0x44
#define BANANAS_KEY_A 0x41

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
    double m_DeltaTime = 0.0;
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

private:
    void KeyboardInput();

};

}