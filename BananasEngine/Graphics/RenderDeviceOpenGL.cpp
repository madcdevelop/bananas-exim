#include "RenderDeviceOpenGL.h"


namespace GraphicsEngine
{

RenderDeviceOpenGL::RenderDeviceOpenGL()
{

}

RenderDeviceOpenGL::~RenderDeviceOpenGL()
{

}

void RenderDeviceOpenGL::Render()
{
    GLCALL(glClearColor(0.3f, 0.3f, 0.3f, 1.0f));
    GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    
    SwapBuffers(m_hDeviceContext);
}

bool RenderDeviceOpenGL::Init()
{
    // Get Device Context and Set Pixel Format
    m_hDeviceContext = GetDC(m_hWnd);

    PIXELFORMATDESCRIPTOR pfd;
    ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;
    int format = ChoosePixelFormat(m_hDeviceContext, &pfd);
    if(!SetPixelFormat(m_hDeviceContext, format, &pfd))
        return MessageBoxA(NULL, "Failed to set pixel format.", "Error", 0);

    // Create OpenGL Render Context
    m_hRenderContext = wglCreateContext(m_hDeviceContext);
    if(!wglMakeCurrent(m_hDeviceContext, m_hRenderContext))
        return MessageBoxA(NULL, "Failed create and activate render context.", "Error", 0);
    
    // Initialize Glad
    if(!gladLoadGL())
        return MessageBoxA(NULL, "Failed to load Glad.", "Error", 0);
    GLCALL(glViewport(0, 0, m_Width, m_Height));

    // Z Buffer Init
    GLCALL(glEnable(GL_DEPTH_TEST));
    GLCALL(glDepthFunc(GL_LESS));

    return true;
}

void RenderDeviceOpenGL::Shutdown()
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(m_hRenderContext);
    ReleaseDC(m_hWnd, m_hDeviceContext);
    m_Running = false;
}

void RenderDeviceOpenGL::Resize()
{
    RECT windowRect = {0};
    if(GetWindowRect(m_hWnd, &windowRect))
    {
        m_Width  = windowRect.right - windowRect.left;
        m_Height = windowRect.bottom - windowRect.top;
        GLCALL(glViewport(0, 0, m_Width, m_Height));
    }
}

}