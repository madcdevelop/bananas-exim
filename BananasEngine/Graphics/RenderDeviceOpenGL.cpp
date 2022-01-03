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
    if(m_scene->m_isModelLoaded == ModelLoadState::FILE_LOADED)
    {
        #if USE_THREAD_TO_LOAD_OPENGL_MESH_TEXTURES_FEATURE_FLAG
            // TODO(neil): sometimes not all meshes are rendered. Data is intact.
            OutputDebugString(L"INFO\t\tFile Loaded! Loading data for Rendering.\n");
            // Make current thread opengl context null
            wglMakeCurrent(NULL, NULL);
            m_LoadDataModelThread = std::thread(&RenderDeviceOpenGL::InitMeshesTextures, this);
            m_LoadDataModelThread.detach();
        #else
            InitMeshesTextures();
        #endif
    }

    if(m_meshTexturesLoaded == 1)
    {
        if(!wglMakeCurrent(m_hDeviceContext, m_hRenderContext))
            MessageBoxA(NULL, "Failed create and activate render context.", "Error", 0);
        m_meshTexturesLoaded = 0;
        m_scene->m_isModelLoaded = ModelLoadState::DATA_LOADED;
    }
    // OpenGL context is checked after the models are loaded so parts of the models are not missing
    else if (wglGetCurrentContext())
    {
        GLCALL(glClearColor(0.3f, 0.3f, 0.3f, 1.0f));
        GLCALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        if (m_scene)
        {
            if (m_scene->m_isModelLoaded == ModelLoadState::DATA_LOADED)
            {
                if (m_scene->m_models.size() > 0)
                {
                    m_scene->Draw((real32)m_width, (real32)m_height);
                }
            }
        }
    }
    
    SwapBuffers(m_hDeviceContext);
}

int32 RenderDeviceOpenGL::InitMeshesTextures()
{
    m_scene->m_isModelLoaded = ModelLoadState::DATA_LOADING;
    
    #if USE_THREAD_TO_LOAD_OPENGL_MESH_TEXTURES_FEATURE_FLAG
        if(!wglMakeCurrent(m_hDeviceContext, m_hRenderContext))
            return MessageBoxA(NULL, "Failed create and activate render context.", "Error", 0);
    #endif

    for(auto& model : m_scene->m_models)
    {
        model.LoadMeshes();
        model.LoadTextures();
    }
    m_meshTexturesLoaded = 1;
    
    #if USE_THREAD_TO_LOAD_OPENGL_MESH_TEXTURES_FEATURE_FLAG
        wglMakeCurrent(NULL, NULL);
    #endif
    
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
    GLCALL(glViewport(0, 0, m_width, m_height));

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
    m_running = false;
}

void RenderDeviceOpenGL::Resize()
{
    RECT windowRect = {0};
    if(GetWindowRect(m_hWnd, &windowRect))
    {
        m_width  = windowRect.right - windowRect.left;
        m_height = windowRect.bottom - windowRect.top;
        GLCALL(glViewport(0, 0, m_width, m_height));
    }
}

}