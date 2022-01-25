#include "../../BananasEngine/Core/Common.h"
#include "../../BananasEngine/Platform/PlatformWin32.h"
#include "../../BananasEngine/Core/Serialize.h"

#include <chrono>
#include <thread>

#ifndef EDITOR_INTERFACE
#define EDITOR_INTERFACE extern "C" __declspec(dllexport)
#endif

// Graphics
PlatformEngine::PlatformWin32* g_window;
GraphicsEngine::Scene* g_scene;

namespace BananasEngineDll
{

EDITOR_INTERFACE
bool Win32CreateWindow(HINSTANCE hInstance, HWND hwnd)
{
    g_window = new PlatformEngine::PlatformWin32(hInstance, hwnd);
    if (g_window) 
    {
        g_window->Win32CreateWindow();
        return true;
    }
    else
        return false;
}

EDITOR_INTERFACE
HWND Win32GetWindowHandle()
{
    return g_window->m_hWnd;
}

EDITOR_INTERFACE
int32 Win32Run()
{
    return g_window->Run();
}

EDITOR_INTERFACE
void Win32Resize()
{
    g_window->m_renderDevice->Resize();
}

EDITOR_INTERFACE
void Win32KeyboardCameraMove()
{
    return g_window->CameraKeyboardCallback();
}

EDITOR_INTERFACE
void Win32Shutdown()
{
    delete g_window;
}

EDITOR_INTERFACE
void CreateScene()
{
    g_scene = new GraphicsEngine::Scene();
    g_window->m_renderDevice->m_scene = g_scene;
}

EDITOR_INTERFACE
void SceneSaveScene(const char* fileName)
{
    auto start = std::chrono::steady_clock::now();

    // CoreEngine::SerializeToXML(std::string(fileName) + ".bxml", g_Scene);
    // CoreEngine::SerializeToJSON(std::string(fileName) + ".bjson", g_Scene);
    CoreEngine::SerializeToYAML(std::string(fileName) + ".byaml", g_scene);

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
    CoreEngine::DeSerializeFromYAML(std::string(fileName) + ".byaml", g_scene);

    auto end = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::string message = "Load Scene\nTime to execute: " + std::to_string(elapsedTime) + " milliseconds\n";
    OutputDebugStringA(message.c_str());
}

EDITOR_INTERFACE
GraphicsEngine::Scene* GetScene()
{
    return g_scene;
} 

EDITOR_INTERFACE
int GetModelLoadState()
{
    int result = (int)g_window->m_renderDevice->m_scene->m_isModelLoaded;
    return result;
}

EDITOR_INTERFACE
void SceneImportModels(const char* fileName)
{
    g_window->m_renderDevice->m_scene->CreateImportThread(std::string(fileName));
}

EDITOR_INTERFACE
void SceneExportModels(const char* fileName)
{
    g_window->m_renderDevice->m_scene->ExportModels(std::string(fileName));
}

EDITOR_INTERFACE
void SceneShutdown()
{
    delete g_scene;
}

EDITOR_INTERFACE
const char* SceneEngineGetModelName()
{
    size_t modelSize = g_window->m_renderDevice->m_scene->m_models.size();
    if (modelSize > 0)
    {
        const char* result = g_window->m_renderDevice->m_scene->m_models[0].m_name.c_str();
        return result;
    }
    return nullptr;
}

EDITOR_INTERFACE
void SceneEngineSetModelName(const char* name)
{
    if (name)
    {
        if (g_window->m_renderDevice->m_scene->m_models.size() > 0)
        {
            g_window->m_renderDevice->m_scene->m_models[0].m_name = std::string(name);
        }
    }
}



}

