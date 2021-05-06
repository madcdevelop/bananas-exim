#include "Renderer.h"


namespace Core
{
Renderer::Renderer(Window* window, Model* model)
    : m_Model(model), m_Camera(glm::vec3(1.0f, 1.0f, 3.0f)), m_Window(window),
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
    m_Model->m_Texture.m_RenderId = m_Model->m_Texture.LoadBMPCustom("C:\\Code\\bananas-exim\\bananas-exim\\content\\textures\\container.bmp");
}

void Renderer::Draw(float timestep)
{
    GLCALL(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
    GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    // Move light around scene
    float lightPosX = 1.5f * sin(timestep);
    float lightPosY = 1.2f;
    float lightPosZ = 2.0f * cos(timestep);
    glm::vec3 lightPos = glm::vec3(lightPosX, lightPosY, lightPosZ);

    // textured cube
    m_Shader1.UseProgram();
    m_Shader1.SetInt("material.diffuse", 0);
    m_Shader1.SetVec3("viewPos", m_Camera.m_Position);

    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
    glm::vec3 diffuseColor = lightColor   * glm::vec3(0.5f);
    glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);

    m_Shader1.SetVec3("light.position", lightPos);
    m_Shader1.SetVec3("light.ambient", ambientColor);
    m_Shader1.SetVec3("light.diffuse", diffuseColor);
    m_Shader1.SetVec3("light.specular", 1.0f, 1.0f, 1.0f);
    m_Shader1.SetVec3("material.ambient",  1.0f, 0.5f, 0.31f);
    m_Shader1.SetVec3("material.specular", 0.5f, 0.5f, 0.5f);
    m_Shader1.SetFloat("material.shininess", 32.0f);

    m_Model->m_VertexBuffer.Bind();
    m_Model->m_IndexBuffer.Bind();
    m_Model->m_Texture.Bind(0);
    GLCALL(glDrawArrays(GL_TRIANGLES, 0, 36));

    // mvp
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::lookAt(m_Camera.m_Position, m_Camera.m_Position + m_Camera.m_Front, m_Camera.m_Up);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)m_Window->m_Width/(float)m_Window->m_Height, 0.1f, 100.0f);
    m_Shader1.SetMatrix4("model", GL_FALSE, model);
    m_Shader1.SetMatrix4("view", GL_FALSE, view);
    m_Shader1.SetMatrix4("projection", GL_FALSE, projection);

    // light cube
    m_ShaderLight.UseProgram();
    model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2f));
    m_ShaderLight.SetMatrix4("model", GL_FALSE, model);
    m_ShaderLight.SetMatrix4("view", GL_FALSE, view);
    m_ShaderLight.SetMatrix4("projection", GL_FALSE, projection);

    GLCALL(glDrawArrays(GL_TRIANGLES, 0, 36));
}

}
