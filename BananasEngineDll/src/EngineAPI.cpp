#include "Common.h"
#include "PlatformWin32.h"

#ifndef EDITOR_INTERFACE
#define EDITOR_INTERFACE extern "C" __declspec(dllexport)
#endif

// Graphics
GraphicsEngine::PlatformWin32* g_Window;
GraphicsEngine::Scene* g_Scene;

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

EDITOR_INTERFACE
int32 Win32Run()
{
    return g_Window->Run();
}

EDITOR_INTERFACE
void CreateScene()
{
    g_Scene = new GraphicsEngine::Scene();
    g_Window->m_RenderDevice->m_Scene = g_Scene;
}

EDITOR_INTERFACE
GraphicsEngine::Scene* GetScene()
{
    return g_Scene;
} 

EDITOR_INTERFACE
void SceneLoadModels(const char* fileName)
{
    g_Window->m_RenderDevice->m_Scene->LoadModels(std::string(fileName));
}

}

EDITOR_INTERFACE
void SceneExportModels(const char* fileName)
{
    g_Window->m_RenderDevice->m_Scene->ExportModels(std::string(fileName));
}