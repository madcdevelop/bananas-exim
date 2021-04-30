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
    m_Model->m_Texture.m_RenderId = m_Model->m_Texture.LoadBMPCustom("C:\\Code\\bananas-exim\\bananas-exim\\content\\textures\\uvtemplate.bmp");
}

void Renderer::Draw()
{
    GLCALL(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
    GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    glm::vec3 lightPos = glm::vec3(1.5f, 1.5f, 2.0f);
    m_Shader1.UseProgram();
    m_Shader1.SetInt1("texture1", 0);
    m_Shader1.SetVec3fv("lightPos", 1, &lightPos[0]);
    m_Shader1.SetVec3fv("lightColor", 1, &glm::vec3(1.2f, 1.2f, 1.2f)[0]);
    m_Shader1.SetVec3fv("viewPos", 1, &m_Camera.m_Position[0]);

    // mvp
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::lookAt(m_Camera.m_Position, m_Camera.m_Position + m_Camera.m_Front, m_Camera.m_Up);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)m_Window->m_Width/(float)m_Window->m_Height, 0.1f, 100.0f);
    m_Shader1.SetMatrix4fv("model", 1, GL_FALSE, &model[0][0]);
    m_Shader1.SetMatrix4fv("view", 1, GL_FALSE, &view[0][0]);
    m_Shader1.SetMatrix4fv("projection", 1, GL_FALSE, &projection[0][0]);

    m_Model->m_VertexBuffer.Bind();
    m_Model->m_IndexBuffer.Bind();
    m_Model->m_Texture.Bind(0);

    GLCALL(glDrawArrays(GL_TRIANGLES, 0, 36));

    m_ShaderLight.UseProgram();
    model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2f));
    m_ShaderLight.SetMatrix4fv("model", 1, GL_FALSE, &model[0][0]);
    m_ShaderLight.SetMatrix4fv("view", 1, GL_FALSE, &view[0][0]);
    m_ShaderLight.SetMatrix4fv("projection", 1, GL_FALSE, &projection[0][0]);

    GLCALL(glDrawArrays(GL_TRIANGLES, 0, 36));
}

}
