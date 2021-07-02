#include "Scene.h"

#include "Importer.h"

// Render data
glm::vec3 g_PointLightPositions[] = {
    glm::vec3(0.0f, 5.0f, 0.0f),
    glm::vec3(-5.0f, 5.0f, -5.0f),
    glm::vec3(-5.0f, 5.0f, 0.0f),
    glm::vec3(0.0f, 5.0f, -5.0f)
};
const unsigned int g_PointLightsCount = sizeof(g_PointLightPositions) / sizeof(*g_PointLightPositions);

const float g_Constant  = 1.0f;
const float g_Linear    = 0.09f;
const float g_Quadratic = 0.032f;

namespace GraphicsEngine
{

Scene::Scene()
    : m_Camera(glm::vec3(5.0f, 10.0f, 10.0f), -120.0f, -30.0f),
      m_Shader1("C:\\Code\\bananas-exim\\Content\\Shaders\\test_vs.glsl", 
                "C:\\Code\\bananas-exim\\Content\\Shaders\\test_fs.glsl"),
      m_ShaderLight("C:\\Code\\bananas-exim\\Content\\Shaders\\lighting_vs.glsl", 
                    "C:\\Code\\bananas-exim\\Content\\Shaders\\lighting_fs.glsl")
{
}

Scene::~Scene()
{

}

void Scene::LoadModels()
{
    // TODO: Loop to add more than 1 model
    Model model1;
    model1.LoadModel("C:\\Code\\bananas-exim\\Content\\Models\\minecraft_hill.obj");
    m_Models.push_back(model1);

    Model lightCube;
    lightCube.LoadModel("C:\\Code\\bananas-exim\\Content\\Models\\cube_example_triangle.obj");
    m_Models.push_back(lightCube);

    for(auto& model : m_Models)
    {
        model.LoadTextures();
    }
}

void Scene::Draw(float screenWidth, float screenHeight)
{
    // Textured cube
    m_Shader1.UseProgram();
    m_Shader1.SetVec3("viewPos", m_Camera.m_Position);

    // Directional light properties
    m_Shader1.SetVec3("dirLight.direction",  m_Camera.m_Front);
    m_Shader1.SetVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
    m_Shader1.SetVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
    m_Shader1.SetVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

    // Point light properties
    for(unsigned int i = 0; i < g_PointLightsCount; i++)
    {
        std::string index = std::to_string(i);
        m_Shader1.SetVec3("pointLights[" + index + "].position", g_PointLightPositions[0]);
        m_Shader1.SetVec3("pointLights[" + index + "].ambient", 0.05f, 0.05f, 0.05f);
        m_Shader1.SetVec3("pointLights[" + index + "].diffuse", 0.8f, 0.8f, 0.8f);
        m_Shader1.SetVec3("pointLights[" + index + "].specular", 1.0f, 1.0f, 1.0f);
        m_Shader1.SetFloat("pointLights[" + index + "].constant", g_Constant);
        m_Shader1.SetFloat("pointLights[" + index + "].linear", g_Linear);
        m_Shader1.SetFloat("pointLights[" + index + "].quadratic", g_Quadratic);
    }

    // Spot light properties
    m_Shader1.SetVec3("spotLight.position", m_Camera.m_Position);
    m_Shader1.SetVec3("spotLight.direction", m_Camera.m_Front);
    m_Shader1.SetVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
    m_Shader1.SetVec3("spotLight.diffuse", 0.8f, 0.8f, 0.8f);
    m_Shader1.SetVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
    m_Shader1.SetFloat("spotLight.cutoff", glm::cos(glm::radians(12.5f)));
    m_Shader1.SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));
    m_Shader1.SetFloat("spotLight.constant", g_Constant);
    m_Shader1.SetFloat("spotLight.linear", g_Linear);
    m_Shader1.SetFloat("spotLight.quadratic", g_Quadratic);

    // Camera
    glm::mat4 view = glm::lookAt(m_Camera.m_Position, m_Camera.m_Position + m_Camera.m_Front, m_Camera.m_Up);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), screenWidth/screenHeight, 0.1f, 100.0f);

    // World transformation
    // TODO: May not need world transform for model imported from blender. 
    //       Vertex Coordinates are already in world space.
    glm::mat4 model = glm::mat4(1.0f);

    // Render Model
    glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(model)));
    glm::mat4 mvp = projection * view * model;
    m_Shader1.SetMatrix4("model", GL_FALSE, model);
    m_Shader1.SetMatrix3("normalMatrix", GL_FALSE, normalMatrix);
    m_Shader1.SetMatrix4("MVP", GL_FALSE, mvp);
    // TODO: replace with function to draw multiple models from scene
    m_Models[0].Draw(m_Shader1);

    // Render Lights
    m_ShaderLight.UseProgram();

    for(unsigned int i = 0; i < g_PointLightsCount; i++)
    {
        glm::mat4 lightModel = glm::mat4(1.0f);
        lightModel = glm::translate(lightModel, g_PointLightPositions[i]);
        lightModel = glm::scale(lightModel, glm::vec3(0.2f));
        glm::mat4 lightMVP = projection * view * lightModel;
        m_ShaderLight.SetMatrix4("MVP", GL_FALSE, lightMVP);
        // TODO: replace with function to draw multiple models from scene
        m_Models[1].Draw(m_ShaderLight);
    }
}

}