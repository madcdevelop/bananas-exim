#include "Renderer.h"


namespace Core
{
Renderer::Renderer(Window* window, Model* model)
    : m_Window(window), m_Model(model), m_Camera(glm::vec3(0.0f, 0.0f, 3.0f)), 
      m_Shader1("C:\\Code\\bananas-exim\\bananas-exim\\content\\test_vs.glsl", 
                "C:\\Code\\bananas-exim\\bananas-exim\\content\\test_fs.glsl")
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
    GLCALL(glClearColor(0.3f, 0.3f, 0.3f, 1.0f));
    GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    GLCALL(glUseProgram(m_Shader1.m_ProgramId));

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)m_Window->m_Width/(float)m_Window->m_Height, 0.1f, 100.0f);
    // glm::mat4 projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 100.0f);
    CameraTransform(projection);

    m_Model->m_VertexBuffer.Bind();
    m_Model->m_IndexBuffer.Bind();
    m_Model->m_Texture.Bind(0);
    GLCALL(glUniform1i(glGetUniformLocation(m_Shader1.m_ProgramId, "texture1"), 0));

    GLCALL(glDrawArrays(GL_TRIANGLES, 0, 36));
}

void Renderer::CameraTransform(glm::mat4 projection)
{
    // Model Matrix : an identity matrix which will be at the origin
    glm::mat4 model = glm::mat4(1.0f);

    // Camera Matrix
    glm::mat4 view = glm::lookAt(m_Camera.m_Position, m_Camera.m_Position + m_Camera.m_Front, m_Camera.m_Up);

    // ModelViewProjection : multiplication of 3 matrices
    glm::mat4 mvp = projection * view * model;

    GLCALL(unsigned int matrixId = glGetUniformLocation(m_Shader1.m_ProgramId, "mvp"));
    GLCALL(glUniformMatrix4fv(matrixId, 1, GL_FALSE, &mvp[0][0]));
}

}
