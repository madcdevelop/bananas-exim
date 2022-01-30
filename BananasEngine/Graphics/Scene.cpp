#include "Scene.h"

#include "Importer.h"
#include "Exporter.h"
#include "../Platform/PlatformWin32.h"

namespace GraphicsEngine
{

// Render data
glm::vec3 g_pointLightPositions[] = {
    glm::vec3(0.0f, 5.0f, 0.0f),
    glm::vec3(-5.0f, 5.0f, -5.0f),
    glm::vec3(-5.0f, 5.0f, 0.0f),
    glm::vec3(0.0f, 5.0f, -5.0f)
};
const uint32 g_pointLightsCount = sizeof(g_pointLightPositions) / sizeof(*g_pointLightPositions);

const real32 g_constant = 1.0f;
const real32 g_linear = 0.09f;
const real32 g_quadratic = 0.032f;

Scene::Scene()
    : m_camera(glm::vec3(5.0f, 10.0f, 10.0f), -120.0f, -30.0f),
      m_shader1("../../../Content/Shaders/test_vs.glsl", 
                "../../../Content/Shaders/test_fs.glsl"),
      m_shaderLight("../../../Content/Shaders/lighting_vs.glsl", 
                    "../../../Content/Shaders/lighting_fs.glsl")
{
}

Scene::~Scene()
{

}

void Scene::CreateImportThread(const std::string& fileName)
{
    OutputDebugString(L"INFO\t\tThread Started!\n");
    m_importThread = std::thread(&Scene::LoadModels, this, fileName);
    OutputDebugString(L"INFO\t\tThread Detached!\n");
    m_importThread.detach();
}

void Scene::LoadModels(const std::string fileName)
{
    if(m_isModelLoaded == ModelLoadState::DATA_LOADED)
        m_isModelLoaded = ModelLoadState::NOT_LOADED;

    // Clear Scene for next import
    if(!m_models.empty())
    {
        m_models.clear();
    }

    Model model1;
    std::string filePathNoExt = fileName.substr(0, fileName.find_last_of("."));
    model1.m_name = filePathNoExt.substr(filePathNoExt.find_last_of("\\")+1);
    model1.LoadModel(fileName);
    m_models.push_back(model1);

    Model lightCube;
    lightCube.m_name = "Light_Cubes";
    std::string filePath = PlatformEngine::PlatformWin32::GetFilePath(L"../../../Content/Models/cube_example.obj");
    lightCube.LoadModel(filePath);
    m_models.push_back(lightCube);

    m_isModelLoaded = ModelLoadState::FILE_LOADED;

    OutputDebugString(L"INFO\t\tThread Ended!\n");
}

void Scene::ExportModels(const std::string& fileName)
{
    Exporter exporter;
    exporter.ExportModel(fileName, m_models);
}


void Scene::Draw(real32 screenWidth, real32 screenHeight)
{
    // Textured cube
    m_shader1.UseProgram();
    m_shader1.SetVec3("viewPos", m_camera.m_position);

    // Directional light properties
    m_shader1.SetVec3("dirLight.direction",  m_camera.m_front);
    m_shader1.SetVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
    m_shader1.SetVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
    m_shader1.SetVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

    // Point light properties
    for(uint32 i = 0; i < g_pointLightsCount; i++)
    {
        std::string index = std::to_string(i);
        m_shader1.SetVec3("pointLights[" + index + "].position", g_pointLightPositions[0]);
        m_shader1.SetVec3("pointLights[" + index + "].ambient", 0.05f, 0.05f, 0.05f);
        m_shader1.SetVec3("pointLights[" + index + "].diffuse", 0.8f, 0.8f, 0.8f);
        m_shader1.SetVec3("pointLights[" + index + "].specular", 1.0f, 1.0f, 1.0f);
        m_shader1.SetFloat("pointLights[" + index + "].constant", g_constant);
        m_shader1.SetFloat("pointLights[" + index + "].linear", g_linear);
        m_shader1.SetFloat("pointLights[" + index + "].quadratic", g_quadratic);
    }

    // Spot light properties
    m_shader1.SetVec3("spotLight.position", m_camera.m_position);
    m_shader1.SetVec3("spotLight.direction", m_camera.m_front);
    m_shader1.SetVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
    m_shader1.SetVec3("spotLight.diffuse", 0.8f, 0.8f, 0.8f);
    m_shader1.SetVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
    m_shader1.SetFloat("spotLight.cutoff", glm::cos(glm::radians(12.5f)));
    m_shader1.SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));
    m_shader1.SetFloat("spotLight.constant", g_constant);
    m_shader1.SetFloat("spotLight.linear", g_linear);
    m_shader1.SetFloat("spotLight.quadratic", g_quadratic);

    // Camera
    glm::mat4 view = glm::lookAt(m_camera.m_position, m_camera.m_position + m_camera.m_front, m_camera.m_up);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), screenWidth/screenHeight, 0.1f, 100.0f);

    // World transformation
    // TODO: May not need world transform for model imported from blender. 
    //       Vertex Coordinates are already in world space.
    glm::mat4 model = glm::mat4(1.0f);

    // Render Model
    glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(model)));
    glm::mat4 mvp = projection * view * model;
    m_shader1.SetMatrix4("model", GL_FALSE, model);
    m_shader1.SetMatrix3("normalMatrix", GL_FALSE, normalMatrix);
    m_shader1.SetMatrix4("MVP", GL_FALSE, mvp);
    // TODO: replace with function to draw multiple models from scene
    m_models[0].Draw(m_shader1);

    // Render Lights
    m_shaderLight.UseProgram();

    for(uint32 i = 0; i < g_pointLightsCount; i++)
    {
        glm::mat4 lightModel = glm::mat4(1.0f);
        lightModel = glm::translate(lightModel, g_pointLightPositions[i]);
        lightModel = glm::scale(lightModel, glm::vec3(0.2f));
        glm::mat4 lightMVP = projection * view * lightModel;
        m_shaderLight.SetMatrix4("MVP", GL_FALSE, lightMVP);
        // TODO: replace with function to draw multiple models from scene
        m_models[1].Draw(m_shaderLight);
    }
}

}
