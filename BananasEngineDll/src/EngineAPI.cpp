#include "Common.h"
#include "PlatformWin32.h"
#include "Serialize.h"

#include <chrono>
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
    auto start = std::chrono::steady_clock::now();

    // CoreEngine::SerializeToXML(std::string(fileName) + ".bxml", g_Scene);
    // CoreEngine::SerializeToJSON(std::string(fileName) + ".bjson", g_Scene);
    CoreEngine::SerializeToYAML(std::string(fileName) + ".byaml", g_Scene);

    auto end = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::string message = "Save Scene\nTime to execute: " + std::to_string(elapsedTime) + " milliseconds\n";
    OutputDebugStringA(message.c_str());
}

EDITOR_INTERFACE
void SceneLoadScene(const char* fileName)
{
    auto start = std::chrono::steady_clock::now();

    // CoreEngine::DeSerializeFromXML(std::string(fileName) + ".bxml", g_Scene);
    // CoreEngine::DeSerializeFromJSON(std::string(fileName) + ".bjson", g_Scene);
    CoreEngine::DeSerializeFromYAML(std::string(fileName) + ".byaml", g_Scene);

    auto end = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::string message = "Load Scene\nTime to execute: " + std::to_string(elapsedTime) + " milliseconds\n";
    OutputDebugStringA(message.c_str());
}

EDITOR_INTERFACE
GraphicsEngine::Scene* GetScene()
{
    return g_Scene;
} 

EDITOR_INTERFACE
void SceneImportModels(const char* fileName)
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