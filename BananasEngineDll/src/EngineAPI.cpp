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
const char* EntityEngineGetModelName()
{
    if (g_window->m_renderDevice->m_scene->m_models.size() > 0)
    {
        const char* result = g_window->m_renderDevice->m_scene->m_models[0].m_name.c_str();
        return result;
    }
    return nullptr;
}

EDITOR_INTERFACE
void EntityEngineSetModelName(const char* name)
{
    if (name)
    {
        if (g_window->m_renderDevice->m_scene->m_models.size() > 0)
        {
            g_window->m_renderDevice->m_scene->m_models[0].m_name = std::string(name);
        }
    }
}

EDITOR_INTERFACE
int EntityEngineGetMeshCount()
{
    int result = (int)g_window->m_renderDevice->m_scene->m_models[0].m_meshes.size();
    return result; 
}

EDITOR_INTERFACE
int EntityEngineGetVerticesCount()
{
    int result = 0;
    auto model = g_window->m_renderDevice->m_scene->m_models[0];
    for (int i = 0; i < model.m_meshes.size(); i++)
    {
        result += (int)model.m_meshes[i].m_vertices.size();
    }
    return result;
}

EDITOR_INTERFACE
int EntityEngineGetIndicesCount()
{
    int result = 0;
    auto model = g_window->m_renderDevice->m_scene->m_models[0];
    for (int i = 0; i < model.m_meshes.size(); i++)
    {
        result += (int)model.m_meshes[i].m_indices.size();
    }
    return result;
}

EDITOR_INTERFACE
const char* EntityEngineGetMeshName(int mIndex)
{
    if (g_window->m_renderDevice->m_scene->m_models.size() > 0)
    {
        const char* result = g_window->m_renderDevice->m_scene->m_models[0].m_meshes[mIndex].m_name.c_str();
        return result;
    }
    return nullptr;
}

EDITOR_INTERFACE
void EntityEngineSetMeshName(const char* name, int mIndex)
{
    if (name)
    {
        if (g_window->m_renderDevice->m_scene->m_models.size() > 0)
        {
            g_window->m_renderDevice->m_scene->m_models[0].m_meshes[mIndex].m_name = std::string(name);
        }
    }
}

EDITOR_INTERFACE
int EntityEngineMeshGetVerticesCount(int mIndex)
{
    auto model = g_window->m_renderDevice->m_scene->m_models[0];
    int result = (int)model.m_meshes[mIndex].m_vertices.size();
    return result;
}

EDITOR_INTERFACE
int EntityEngineMeshGetIndicesCount(int mIndex)
{
    auto model = g_window->m_renderDevice->m_scene->m_models[0];
    int result = (int)model.m_meshes[mIndex].m_indices.size();
    return result;
}

EDITOR_INTERFACE
const char* EntityEngineMeshGetMaterialName(int mIndex)
{
    if (g_window->m_renderDevice->m_scene->m_models.size() > 0)
    {
        const char* result = g_window->m_renderDevice->m_scene->m_models[0].m_meshes[mIndex].m_material.m_name.c_str();
        return result;
    }
    return nullptr;
}

EDITOR_INTERFACE
void EntityEngineMeshSetMaterialName(const char* name, int mIndex)
{
    if (name)
    {
        if (g_window->m_renderDevice->m_scene->m_models.size() > 0)
        {
            g_window->m_renderDevice->m_scene->m_models[0].m_meshes[mIndex].m_material.m_name = std::string(name);
        }
    }
}

EDITOR_INTERFACE
const glm::vec3* EntityEngineMaterialGetAmbient(int mIndex)
{
    auto result = &(g_window->m_renderDevice->m_scene->m_models[0].m_meshes[mIndex].m_material.m_ambient);
    return result;
}

EDITOR_INTERFACE
const void EntityEngineMaterialSetAmbient(int mIndex, const glm::vec3* ambient)
{
    g_window->m_renderDevice->m_scene->m_models[0].m_meshes[mIndex].m_material.m_ambient = *ambient;
}

EDITOR_INTERFACE
const glm::vec3* EntityEngineMaterialGetDiffuse(int mIndex)
{
    auto result = &(g_window->m_renderDevice->m_scene->m_models[0].m_meshes[mIndex].m_material.m_diffuse);
    return result;
}

EDITOR_INTERFACE
const void EntityEngineMaterialSetDiffuse(int mIndex, const glm::vec3* diffuse)
{
    g_window->m_renderDevice->m_scene->m_models[0].m_meshes[mIndex].m_material.m_diffuse = *diffuse;
}

EDITOR_INTERFACE
const glm::vec3* EntityEngineMaterialGetSpecular(int mIndex)
{
    auto result = &(g_window->m_renderDevice->m_scene->m_models[0].m_meshes[mIndex].m_material.m_specular);
    return result;
}

EDITOR_INTERFACE
const void EntityEngineMaterialSetSpecular(int mIndex, const glm::vec3* specular)
{
    g_window->m_renderDevice->m_scene->m_models[0].m_meshes[mIndex].m_material.m_specular = *specular;
}

EDITOR_INTERFACE
const glm::vec3* EntityEngineMaterialGetEmissive(int mIndex)
{
    auto result = &(g_window->m_renderDevice->m_scene->m_models[0].m_meshes[mIndex].m_material.m_emissive);
    return result;
}

EDITOR_INTERFACE
const void EntityEngineMaterialSetEmissive(int mIndex, const glm::vec3* emissive)
{
    g_window->m_renderDevice->m_scene->m_models[0].m_meshes[mIndex].m_material.m_emissive = *emissive;
}

EDITOR_INTERFACE
float EntityEngineMaterialGetShininess(int mIndex)
{
    float result = g_window->m_renderDevice->m_scene->m_models[0].m_meshes[mIndex].m_material.m_shininess;
    return result;
}

EDITOR_INTERFACE
void EntityEngineMaterialSetShininess(int mIndex, float shininess)
{
    g_window->m_renderDevice->m_scene->m_models[0].m_meshes[mIndex].m_material.m_shininess = shininess;
}

EDITOR_INTERFACE
int EntityEngineMaterialGetTextureSize(int mIndex)
{
    int result = (int)g_window->m_renderDevice->m_scene->m_models[0].m_meshes[mIndex].m_material.m_textures.size();
    return result;
}

EDITOR_INTERFACE
const char* EntityEngineMaterialTextureType(int mIndex, int textureIndex)
{
    const char* result = g_window->m_renderDevice->m_scene->m_models[0].m_meshes[mIndex].m_material.m_textures[textureIndex].m_type.c_str();
    return result;
}

EDITOR_INTERFACE
const char* EntityEngineMaterialTextureFilePath(int mIndex, int textureIndex)
{
    const char* result = g_window->m_renderDevice->m_scene->m_models[0].m_meshes[mIndex].m_material.m_textures[textureIndex].m_filePath.c_str();
    return result;
}

}
