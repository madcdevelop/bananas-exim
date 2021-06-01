#include "Window.h"

#include <vector>

#include "Renderer.h"
#include "RendererTestData.h"

namespace
{
    Core::Window* g_Window = nullptr;
    Core::VertexBuffer* g_vbo = nullptr;
    Core::IndexBuffer* g_ibo = nullptr;
    Core::Texture* g_tex = nullptr;
    std::vector<Core::Texture>* g_textures = nullptr;
    Core::Mesh* g_Mesh = nullptr;
    Core::Renderer* g_RenderOpenGL = nullptr;
    Core::Timestep* g_Timestep = nullptr;
}

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if(g_Window)
        return g_Window->MsgProc(hwnd, msg, wParam, lParam);
    else
        return DefWindowProc(hwnd, msg, wParam, lParam);
}

namespace Core
{

Window::Window(HINSTANCE hInstance, HWND hwnd)
    : m_hInstance(hInstance), m_hWnd(hwnd), m_hDeviceContext(NULL), m_hRenderContext(NULL), 
      m_Width(800), m_Height(600), m_WindowTitle(nullptr), 
      m_WindowStyle(WS_VISIBLE)
{
    g_Window = this;
    m_LastX =  m_Width  / 2.0f;
    m_LastY =  m_Height / 2.0f;
}

Window::~Window()
{
}

int Window::Run()
{
    MSG msg = {0};
    g_Timestep = new Timestep;
    g_Timestep->StartCounter();
    while(WM_QUIT != msg.message)
    {
        double time = g_Timestep->GetTime();
        m_DeltaTime = time - m_LastFrameTime;
        //g_Timestep->Print(m_LastFrameTime, time, deltaTime);
        m_LastFrameTime = time;

        if(PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            Render();
            SwapBuffers(m_hDeviceContext);
        }
    }

    Shutdown();

    return static_cast<int>(msg.wParam);
}

bool Window::InitWindow()
{
    WNDCLASS wc;
    ZeroMemory(&wc, sizeof(WNDCLASS));
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.hInstance = m_hInstance;
    wc.lpfnWndProc = MainWndProc;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszClassName = L"WindowClass";
    wc.lpszMenuName = NULL;

    if(!RegisterClass(&wc)) MessageBoxA(NULL, "The window class failed to register.", "Error", 0);

    // Adjust Window Rect for Client Size
    RECT windowRect = {0, 0, (LONG)m_Width, (LONG)m_Height};
    AdjustWindowRect((LPRECT)&wc, m_WindowStyle, FALSE);
    int width  = windowRect.right - windowRect.left;
    int height = windowRect.bottom - windowRect.top;
    int x = GetSystemMetrics(SM_CXSCREEN)/2 - width/2;
    int y = GetSystemMetrics(SM_CYSCREEN)/2 - height/2;

    // Create Window
    m_WindowStyle |=  m_hWnd ? WS_CHILD : WS_OVERLAPPEDWINDOW;
    m_hWnd = CreateWindow(wc.lpszClassName, m_WindowTitle, m_WindowStyle, x, y, width, height, m_hWnd, NULL, m_hInstance, NULL);
    if(!m_hWnd) MessageBoxA(NULL, "Failed to create window.", "Error", 0);

    // Show Window
    if (m_hWnd) {
        SetWindowLongPtr(m_hWnd, GWL_STYLE, m_WindowStyle);
        ShowWindow(m_hWnd, m_WindowStyle);
    }

    return true;
}

bool Window::InitGL()
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

    // Init Rendering

    // Convert to Vertex and Index buffer to vectors
    std::vector<Vertex> outVertices;
    int verticesLength = sizeof(vertices)/sizeof(*vertices);

    for(int i = 0; i < verticesLength; (i+=8))
    {
        Vertex vertex;
        vertex.position   = glm::vec3(vertices[i], vertices[i+1], vertices[i+2]);
        vertex.normal     = glm::vec3(vertices[i+3], vertices[i+4], vertices[i+5]);
        vertex.textureUV  = glm::vec2(vertices[i+6], vertices[i+7]);
        outVertices.push_back(vertex);
    }

    std::vector<unsigned int> outIndices;
    std::copy(std::begin(indices), std::end(indices), std::back_inserter(outIndices));

    g_vbo = new Core::VertexBuffer{ outVertices };
    g_ibo = new Core::IndexBuffer{ outIndices };
    g_textures = new std::vector<Texture>();
    g_textures->push_back(Core::Texture("texture_diffuse"));
    g_textures->push_back(Core::Texture("texture_specular"));
    g_Mesh = new Core::Mesh{ "Cube", *g_vbo, *g_ibo, *g_textures };
    g_RenderOpenGL = new Core::Renderer{this, g_Mesh};
    
    return true;
}

bool Window::Init()
{
    if(!InitWindow()) return false;
    if(!InitGL()) return false;

    return true;
}

LRESULT Window::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
    case WM_KEYDOWN:
        {
            CameraKeyboardCallback();
        } break;
    case WM_MOUSEMOVE:
        {
            if(GetAsyncKeyState(VK_MBUTTON) & 0x8000)
            {
                POINT pos{ 0, 0 };
                if (GetCursorPos(&pos))
                {
                    CameraMouseCallback(pos);
                }
            }
            else {
                m_FirstMouse = true;
            }
        } break;
    case WM_SIZE:
        {
            // skip first call to resize window
            if (m_FirstResize) ResizeWindowCallback();
            else m_FirstResize = true;
            
            return 0;
        } break;
    case WM_CLOSE:
        {
            DestroyWindow(hwnd);
            return 0;
        } break;
    case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        } break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

void Window::Render()
{
    g_RenderOpenGL->Draw((float)g_Timestep->GetTime());
}

void Window::Shutdown()
{
    // Delete Rendering objects
    delete g_vbo;
    delete g_ibo;
    delete g_textures;
    delete g_Mesh;
    delete g_RenderOpenGL;
    delete g_Timestep;

    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(m_hRenderContext);
    ReleaseDC(m_hWnd, m_hDeviceContext);
}

void Window::CameraKeyboardCallback()
{
    if(GetAsyncKeyState(BANANAS_KEY_W) & 0x8000)
        g_RenderOpenGL->m_Camera.KeyboardMovement(Core::CameraMovement::FORWARD, (float)m_DeltaTime);
    if(GetAsyncKeyState(BANANAS_KEY_S) & 0x8000)
        g_RenderOpenGL->m_Camera.KeyboardMovement(Core::CameraMovement::BACKWARD, (float)m_DeltaTime);
    if(GetAsyncKeyState(BANANAS_KEY_A) & 0x8000)
        g_RenderOpenGL->m_Camera.KeyboardMovement(Core::CameraMovement::LEFT, (float)m_DeltaTime);
    if(GetAsyncKeyState(BANANAS_KEY_D) & 0x8000)
        g_RenderOpenGL->m_Camera.KeyboardMovement(Core::CameraMovement::RIGHT, (float)m_DeltaTime);
}

void Window::CameraMouseCallback(const POINT& pos)
{
    if(m_FirstMouse)
    {
        m_LastX = (float)pos.x;
        m_LastY = (float)pos.y;
        m_FirstMouse = false;
    }

    float xoffset = (float)pos.x - m_LastX;
    float yoffset = m_LastY - (float)pos.y;
    m_LastX = (float)pos.x;
    m_LastY = (float)pos.y;

    g_RenderOpenGL->m_Camera.MouseMovement(xoffset, yoffset);
}

void Window::ResizeWindowCallback()
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