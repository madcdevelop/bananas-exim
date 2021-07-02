#include "Renderer.h"



namespace GraphicsEngine
{

Renderer::Renderer(HINSTANCE hInstance)
    : m_hInstance(hInstance)
{
}

Renderer::~Renderer()
{
    GLCALL(glDisableVertexAttribArray(0));
    GLCALL(glDisableVertexAttribArray(1));
    GLCALL(glDisableVertexAttribArray(2));
}

void Renderer::CreateRenderDevice(RenderDeviceOpenGL* renderDevice)
{
    if(!renderDevice)
    {
        m_Device = new RenderDeviceOpenGL();
    }
}

void Renderer::Draw(float timestep)
{
    GLCALL(glClearColor(0.3f, 0.3f, 0.3f, 1.0f));
    GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

}

}
