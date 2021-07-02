#include "Common.h"
#include "PlatformWin32.h"

#ifndef EDITOR_INTERFACE
#define EDITOR_INTERFACE extern "C" __declspec(dllexport)
#endif

// Graphics
GraphicsEngine::PlatformWin32* g_Window;

namespace BananasEngineDll
{

EDITOR_INTERFACE
bool Win32CreateWindow(HINSTANCE hInstance, HWND hwnd)
{
    g_Window = new GraphicsEngine::PlatformWin32(hInstance, hwnd);
    if (g_Window) 
    {
        g_Window->Win32CreateWindow();
        return true;
    }
    else
        return false;
}

EDITOR_INTERFACE
HWND Win32GetWindowHandle()
{
    return g_Window->m_hWnd;
}

}