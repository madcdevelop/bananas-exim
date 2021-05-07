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
    m_Model->m_Textures[0].m_RenderId = m_Model->m_Textures[0].LoadBMPCustom("C:\\Code\\bananas-exim\\bananas-exim\\content\\textures\\container.bmp");
    m_Model->m_Textures[1].m_RenderId = m_Model->m_Textures[1].LoadBMPCustom("C:\\Code\\bananas-exim\\bananas-exim\\content\\textures\\container_specular.bmp");
}

void Renderer::Draw(float timestep)
{
    GLCALL(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
    GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

    // textured cube
    m_Shader1.UseProgram();
    m_Shader1.SetInt("material.diffuse", 0);
    m_Shader1.SetInt("material.specular", 1);
    m_Shader1.SetVec3("viewPos", m_Camera.m_Position);

    glm::vec3 lightColor(1.0f, 1.0f, 1.0f);
    glm::vec3 diffuseColor = lightColor   * glm::vec3(0.5f);
    glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);

    m_Shader1.SetVec3("light.position", lightPos);
    // m_Shader1.SetVec3("light.direction", -0.2f, -1.0f, -0.3f);
    m_Shader1.SetVec3("light.ambient", ambientColor);
    m_Shader1.SetVec3("light.diffuse", diffuseColor);
    m_Shader1.SetVec3("light.specular", 1.0f, 1.0f, 1.0f);
    m_Shader1.SetVec3("material.ambient",  1.0f, 0.5f, 0.31f);
    m_Shader1.SetFloat("material.shininess", 32.0f);
    m_Shader1.SetFloat("light.constant", 1.0f);
    m_Shader1.SetFloat("light.linear", 0.09f);
    m_Shader1.SetFloat("light.quadratic", 0.032f);

    m_Model->m_VertexBuffer.Bind();
    m_Model->m_IndexBuffer.Bind();
    m_Model->m_Textures[0].Bind(0);
    m_Model->m_Textures[1].Bind(1);

    for(unsigned int i = 0; i < 10; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[i]);
        float angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        m_Shader1.SetMatrix4("model", GL_FALSE, model);

        GLCALL(glDrawArrays(GL_TRIANGLES, 0, 36));
    }
    
    // camera
    glm::mat4 view = glm::lookAt(m_Camera.m_Position, m_Camera.m_Position + m_Camera.m_Front, m_Camera.m_Up);
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)m_Window->m_Width/(float)m_Window->m_Height, 0.1f, 100.0f);
    m_Shader1.SetMatrix4("view", GL_FALSE, view);
    m_Shader1.SetMatrix4("projection", GL_FALSE, projection);

    // light cube
    m_ShaderLight.UseProgram();
    glm::mat4 lightModel = glm::mat4(1.0f);
    lightModel = glm::translate(lightModel, lightPos);
    lightModel = glm::scale(lightModel, glm::vec3(0.2f));
    m_ShaderLight.SetMatrix4("model", GL_FALSE, lightModel);
    m_ShaderLight.SetMatrix4("view", GL_FALSE, view);
    m_ShaderLight.SetMatrix4("projection", GL_FALSE, projection);

    GLCALL(glDrawArrays(GL_TRIANGLES, 0, 36));
}

}
