#include "Window.h"
#include "Renderer.h"

namespace
{
    Core::Window* g_Window = NULL;
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
}

Window::~Window()
{
}

int Window::Run()
{
    MSG msg = {0};
    while(WM_QUIT != msg.message)
    {
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
    glViewport(0, 0, m_Width, m_Height);
    
    // Z Buffer Init
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    
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
    // Create Cube
    static const GLfloat vertexBufferData[] = {
        -1.0f,-1.0f,-1.0f, // triangle 1 : begin
        -1.0f,-1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f, // triangle 1 : end
        1.0f, 1.0f,-1.0f, // triangle 2 : begin
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f, // triangle 2 : end
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f
    };

    // One color for each vertex. They were generated randomly.
    static const GLfloat colorBufferData[] = {
        0.583f,  0.771f,  0.014f,
        0.609f,  0.115f,  0.436f,
        0.327f,  0.483f,  0.844f,
        0.822f,  0.569f,  0.201f,
        0.435f,  0.602f,  0.223f,
        0.310f,  0.747f,  0.185f,
        0.597f,  0.770f,  0.761f,
        0.559f,  0.436f,  0.730f,
        0.359f,  0.583f,  0.152f,
        0.483f,  0.596f,  0.789f,
        0.559f,  0.861f,  0.639f,
        0.195f,  0.548f,  0.859f,
        0.014f,  0.184f,  0.576f,
        0.771f,  0.328f,  0.970f,
        0.406f,  0.615f,  0.116f,
        0.676f,  0.977f,  0.133f,
        0.971f,  0.572f,  0.833f,
        0.140f,  0.616f,  0.489f,
        0.997f,  0.513f,  0.064f,
        0.945f,  0.719f,  0.592f,
        0.543f,  0.021f,  0.978f,
        0.279f,  0.317f,  0.505f,
        0.167f,  0.620f,  0.077f,
        0.347f,  0.857f,  0.137f,
        0.055f,  0.953f,  0.042f,
        0.714f,  0.505f,  0.345f,
        0.783f,  0.290f,  0.734f,
        0.722f,  0.645f,  0.174f,
        0.302f,  0.455f,  0.848f,
        0.225f,  0.587f,  0.040f,
        0.517f,  0.713f,  0.338f,
        0.053f,  0.959f,  0.120f,
        0.393f,  0.621f,  0.362f,
        0.673f,  0.211f,  0.457f,
        0.820f,  0.883f,  0.371f,
        0.982f,  0.099f,  0.879f
    };

    VertexBuffer vbo(vertexBufferData, sizeof(vertexBufferData));
    IndexBuffer ibo(colorBufferData, sizeof(colorBufferData));
    Texture texture("C:\\Code\\bananas-exim\\bananas-exim\\content\\textures\\uvtemplate.bmp");

    Renderer renderOpenGL{this, &vbo, &ibo, &texture};

    renderOpenGL.Draw();
}

void Window::Shutdown()
{
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(m_hRenderContext);
    ReleaseDC(m_hWnd, m_hDeviceContext);
}

}