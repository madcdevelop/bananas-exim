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
    DATA_LOADING = 2,
    DATA_LOADED = 3
};

class Scene
{
public:
    std::vector<Model> m_models;
    Camera m_camera;

    ModelLoadState m_modelLoadState = ModelLoadState::NOT_LOADED;
    std::thread m_importThread;

private:
    Shader m_shader1;
    Shader m_shaderLight;


public:
    Scene();
    ~Scene();

    void CreateImportThread(const std::string& fileName);

    void LoadModels(const std::string fileName);
    void ExportModels(const std::string& fileName);
    void Draw(real32 screenWidth, real32 screenHeight);


};

}

#endif
