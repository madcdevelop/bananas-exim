#ifndef GRAPHICS_ENGINE_SCENE_H
#define GRAPHICS_ENGINE_SCENE_H

#include "../Core/Common.h"
#include "Camera.h"
#include "Model.h"
#include "Shader.h"

#include <thread>

namespace GraphicsEngine
{

enum ModelLoadState {
    NOT_LOADED  = 0,
    FILE_LOADED = 1,
    DATA_LOADED = 2
};

class Scene
{
public:
    std::vector<Model> m_Models;
    Camera m_Camera;

    ModelLoadState m_IsModelLoaded = ModelLoadState::NOT_LOADED;
    std::thread m_ImportThread;

private:
    Shader m_Shader1;
    Shader m_ShaderLight;


public:
    Scene();
    ~Scene();

    void CreateImportThread(const std::string& fileName);

    void LoadModels(const std::string fileName);
    void ExportModels(const std::string& fileName);
    void Draw(float screenWidth, float screenHeight);


};

}

#endif