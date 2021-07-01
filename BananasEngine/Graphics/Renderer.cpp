#include "Renderer.h"



namespace GraphicsEngine
{

Renderer::Renderer(Window* window)
    : m_Window(window)
{
    Init();
}

Renderer::~Renderer()
{
    GLCALL(glDisableVertexAttribArray(0));
    GLCALL(glDisableVertexAttribArray(1));
    GLCALL(glDisableVertexAttribArray(2));
    delete m_Scene;
}

void Renderer::Init()
{
    // Z Buffer Init
    GLCALL(glEnable(GL_DEPTH_TEST));
    GLCALL(glDepthFunc(GL_LESS));

    // Load Scene
    m_Scene = new GraphicsEngine::Scene{};
    m_Scene->LoadModels();
}

void Renderer::Draw(float timestep)
{
    GLCALL(glClearColor(0.3f, 0.3f, 0.3f, 1.0f));
    GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    if(m_Scene->m_Models.size() > 0)
        m_Scene->Draw(timestep, m_Window);

}

}
