#include "RenderDeviceOpenGL.h"


namespace GraphicsEngine
{

RenderDeviceOpenGL::RenderDeviceOpenGL()
{

}

RenderDeviceOpenGL::~RenderDeviceOpenGL()
{
    GLCALL(glDisableVertexAttribArray(0));
    GLCALL(glDisableVertexAttribArray(1));
    GLCALL(glDisableVertexAttribArray(2));
}

void RenderDeviceOpenGL::Render()
{
    // Loading data for Rendering
    if(m_Scene->m_IsModelLoaded == ModelLoadState::FILE_LOADED)
    {
        OutputDebugString(L"INFO\t\tFile Loaded! Loading data for Rendering.\n");
        // Make current thread opengl context null
        wglMakeCurrent(NULL, NULL);
        m_LoadDataModelThread = std::thread(&RenderDeviceOpenGL::InitMeshesTextures, this);
        m_LoadDataModelThread.detach();
    }

    if(m_MeshTexturesLoaded == 1)
    {
        if(!wglMakeCurrent(m_hDeviceContext, m_hRenderContext))
            MessageBoxA(NULL, "Failed create and activate render context.", "Error", 0);
        m_MeshTexturesLoaded = 0;
        m_Scene->m_IsModelLoaded = ModelLoadState::DATA_LOADED;
    }
    // OpenGL context is checked after the models are loaded so parts of the models are not missing
    else if (wglGetCurrentContext())
    {
        GLCALL(glClearColor(0.3f, 0.3f, 0.3f, 1.0f));
        GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        if (m_Scene)
        {
            if (m_Scene->m_IsModelLoaded == ModelLoadState::DATA_LOADED)
            {
                if (m_Scene->m_Models.size() > 0)
                {
                    // TODO(neil): sometimes not all meshes are rendered.
                    m_Scene->Draw((float)m_Width, (float)m_Height);
                }
            }
        }
    }
    
    SwapBuffers(m_hDeviceContext);
}

int32 RenderDeviceOpenGL::InitMeshesTextures()
{
    m_Scene->m_IsModelLoaded = ModelLoadState::DATA_LOADING;
    if(!wglMakeCurrent(m_hDeviceContext, m_hRenderContext))
        return MessageBoxA(NULL, "Failed create and activate render context.", "Error", 0);

    for(auto& model : m_Scene->m_Models)
    {
        model.LoadMeshes();
        model.LoadTextures();
    }
    m_MeshTexturesLoaded = 1;
    wglMakeCurrent(NULL, NULL);
    return 1;
}

bool RenderDeviceOpenGL::Init()
{
    // Get Device Context and Set Pixel Format
    ASSERT(m_hWnd != nullptr)
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
    int32 format = ChoosePixelFormat(m_hDeviceContext, &pfd);
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