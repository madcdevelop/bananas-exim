#include "Renderer.h"



namespace GraphicsEngine
{

Renderer::Renderer(HINSTANCE hInstance)
    : m_hInstance(hInstance)
{
}

Renderer::~Renderer()
{
    delete m_device;
}

void Renderer::CreateRenderDevice(RenderDeviceOpenGL* renderDevice)
{
    if(!renderDevice)
    {
        m_device = new RenderDeviceOpenGL();
    }
}

void Renderer::Draw(real32 timestep)
{
    GLCALL(glClearColor(0.3f, 0.3f, 0.3f, 1.0f));
    GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

}

}
