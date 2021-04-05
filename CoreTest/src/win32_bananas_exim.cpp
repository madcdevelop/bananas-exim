#ifndef UNICODE
#define UNICODE
#endif

#include "Renderer.h"
#include "Window.h"

// Entry point to program
int APIENTRY 
WinMain(HINSTANCE hInstance,
        HINSTANCE hPrevInstance,
        LPSTR     lpCmdLine,
        int       nShowCmd)
{
    Core::Window window{ hInstance, NULL };
    if(!window.Init())
        return 1;
    
    return window.Run();
}