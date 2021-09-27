#include "Common.h"
#include "PlatformWin32.h"
#include "Serialize.h"

#include <thread>

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
void Win32Resize()
{
    g_Window->m_RenderDevice->Resize();
}

EDITOR_INTERFACE
void Win32KeyboardCameraMove()
{
    return g_Window->CameraKeyboardCallback();
}

EDITOR_INTERFACE
void Win32Shutdown()
{
    delete g_Window;
}

EDITOR_INTERFACE
void CreateScene()
{
    g_Scene = new GraphicsEngine::Scene();
    g_Window->m_RenderDevice->m_Scene = g_Scene;
}

EDITOR_INTERFACE
void SceneSaveScene(const char* fileName)
{
    CoreEngine::SerializeToXML(fileName, g_Scene);
}

EDITOR_INTERFACE
void SceneOpenScene(const char* fileName)
{
    CoreEngine::DeSerializeFromXML(fileName, g_Scene);
}

EDITOR_INTERFACE
GraphicsEngine::Scene* GetScene()
{
    return g_Scene;
} 

EDITOR_INTERFACE
void SceneLoadModels(const char* fileName)
{
    g_Window->m_RenderDevice->m_Scene->CreateImportThread(std::string(fileName));
}

}

EDITOR_INTERFACE
void SceneExportModels(const char* fileName)
{
    g_Window->m_RenderDevice->m_Scene->ExportModels(std::string(fileName));
}

EDITOR_INTERFACE
void SceneShutdown()
{
    delete g_Scene;
}