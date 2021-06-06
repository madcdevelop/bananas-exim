#include "Renderer.h"

// Render data
glm::vec3 g_PointLightPositions[] = {
    glm::vec3(-3.7f,  0.2f,  2.0f),
    glm::vec3( 2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f,  2.0f, -12.0f),
    glm::vec3( 0.0f,  0.0f, -3.0f)
};
const unsigned int g_PointLightsCount = sizeof(g_PointLightPositions) / sizeof(*g_PointLightPositions);

const float g_Constant  = 1.0f;
const float g_Linear    = 0.09f;
const float g_Quadratic = 0.032f;

namespace Core
{

Renderer::Renderer(Window* window, Mesh* mesh)
    : m_Mesh(mesh), m_Camera(glm::vec3(1.0f, 7.0f, 10.0f), -90.0f, -30.0f), m_Window(window),
      m_Shader1("C:\\Code\\bananas-exim\\bananas-exim\\content\\test_vs.glsl", 
                "C:\\Code\\bananas-exim\\bananas-exim\\content\\test_fs.glsl"),
      m_ShaderLight("C:\\Code\\bananas-exim\\bananas-exim\\content\\lighting_vs.glsl", 
                    "C:\\Code\\bananas-exim\\bananas-exim\\content\\lighting_fs.glsl")
{
    Init();
}

Renderer::~Renderer()
{
    GLCALL(glDisableVertexAttribArray(0));
    GLCALL(glDisableVertexAttribArray(1));
}

void Renderer::Init()
{
    // Z Buffer Init
    GLCALL(glEnable(GL_DEPTH_TEST));
    GLCALL(glDepthFunc(GL_LESS));

    // Load Textures
    m_Mesh->m_Textures[0].m_RenderId = m_Mesh->m_Textures[0].LoadBMPCustom("C:\\Code\\bananas-exim\\bananas-exim\\content\\textures\\minecraft_cube_texture.bmp");
    m_Mesh->m_Textures[1].m_RenderId = m_Mesh->m_Textures[1].LoadBMPCustom("C:\\Code\\bananas-exim\\bananas-exim\\content\\textures\\minecraft_cube_texture.bmp");
}

void Renderer::Draw(float timestep)
{
    GLCALL(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
    GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    // Textured cube
    m_Shader1.UseProgram();
    m_Shader1.SetVec3("viewPos", m_Camera.m_Position);
    m_Shader1.SetFloat("material.shininess", 32.0f);

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
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)m_Window->m_Width/(float)m_Window->m_Height, 0.1f, 100.0f);

    // World transformation
    // @TODO: May not need world transform for model imported from blender. 
    //        Vertex Coordinates are already in world space.
    glm::mat4 model = glm::mat4(1.0f);

    // Bind buffers
    m_Mesh->m_VertexBuffer.Bind();
    m_Mesh->m_IndexBuffer.Bind();

    // Render Model
    glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(model)));
    glm::mat4 mvp = projection * view * model;
    m_Shader1.SetMatrix4("model", GL_FALSE, model);
    m_Shader1.SetMatrix3("normalMatrix", GL_FALSE, normalMatrix);
    m_Shader1.SetMatrix4("MVP", GL_FALSE, mvp);
    m_Mesh->Draw(m_Shader1);

    // Render Lights
    m_ShaderLight.UseProgram();

    for(unsigned int i = 0; i < g_PointLightsCount; i++)
    {
        glm::mat4 lightModel = glm::mat4(1.0f);
        lightModel = glm::translate(lightModel, g_PointLightPositions[i]);
        lightModel = glm::scale(lightModel, glm::vec3(0.2f));
        glm::mat4 lightMVP = projection * view * lightModel;
        m_ShaderLight.SetMatrix4("MVP", GL_FALSE, lightMVP);
        
        m_Mesh->Draw(m_ShaderLight);
    }

}

}
