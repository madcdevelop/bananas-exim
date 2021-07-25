#include "PlatformWin32.h"

namespace
{
    GraphicsEngine::PlatformWin32 *g_PlatformWin32;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if(g_PlatformWin32)
        return g_PlatformWin32->MsgProc(hwnd, msg, wParam, lParam);
    else
        return DefWindowProc(hwnd, msg, wParam, lParam);
}

namespace GraphicsEngine
{

PlatformWin32::PlatformWin32(HINSTANCE hInstance, HWND hwnd)
    : m_hInstance(hInstance), m_hWnd(hwnd), m_hDeviceContext(NULL), m_hRenderContext(NULL), 
      m_Width(800), m_Height(600), m_WindowTitle(L"Bananas Export/Import"), 
      m_WindowStyle(WS_VISIBLE), m_Renderer(NULL), m_RenderDevice(NULL)
{
    g_PlatformWin32 = this;
    m_Timestep = new CoreEngine::Timestep();
    m_Timestep->StartCounter();
}

PlatformWin32::~PlatformWin32()
{
}

bool PlatformWin32::Win32CreateWindow()
{
    // Initialize PlatformWin32
    Init();

    // TODO(neil): Move this section out of function and into EngineDll.
    // Initialize Renderer
    m_Renderer = new Renderer{m_hInstance};
    m_Renderer->CreateRenderDevice(m_RenderDevice);

    // Initialize RenderDevice
    m_RenderDevice = m_Renderer->m_Device;
    m_RenderDevice->m_hWnd    = m_hWnd;
    m_RenderDevice->m_Width   = m_Width;
    m_RenderDevice->m_Height  = m_Height;
    m_RenderDevice->m_Running = true;
    m_RenderDevice->Init();

    return true;    
}

bool PlatformWin32::Init()
{
    WNDCLASS wc;
    ZeroMemory(&wc, sizeof(WNDCLASS));
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.hInstance = m_hInstance;
    wc.lpfnWndProc = WndProc;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszClassName = L"WindowClass";
    wc.lpszMenuName = NULL;

    if(!RegisterClass(&wc)) MessageBoxA(NULL, "The window class failed to register.", "Error", 0);

    // Adjust Window Rect for Client Size
    RECT windowRect = {0, 0, (LONG)m_Width, (LONG)m_Height};
    AdjustWindowRect((LPRECT)&wc, m_WindowStyle, FALSE);
    int32 width  = windowRect.right - windowRect.left;
    int32 height = windowRect.bottom - windowRect.top;
    int32 x = GetSystemMetrics(SM_CXSCREEN)/2 - width/2;
    int32 y = GetSystemMetrics(SM_CYSCREEN)/2 - height/2;

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

int32 PlatformWin32::Run()
{
    // Timestep
    double time = m_Timestep->GetTime();
    m_DeltaTime = time - m_LastFrameTime;
    m_LastFrameTime = time;

    MSG message;
    while(PeekMessage(&message, NULL, NULL, NULL, PM_REMOVE))
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
    if(m_RenderDevice)
    {
        m_RenderDevice->Render();
    }
    return 0;
}

LRESULT PlatformWin32::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
    case WM_KEYDOWN:
        {
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

void PlatformWin32::CameraKeyboardCallback()
{
    if(GetAsyncKeyState(BANANAS_KEY_W) & 0x8000)
        m_RenderDevice->m_Scene->m_Camera.KeyboardMovement(GraphicsEngine::CameraMovement::FORWARD, (float)m_DeltaTime);
    if(GetAsyncKeyState(BANANAS_KEY_S) & 0x8000)
        m_RenderDevice->m_Scene->m_Camera.KeyboardMovement(GraphicsEngine::CameraMovement::BACKWARD, (float)m_DeltaTime);
    if(GetAsyncKeyState(BANANAS_KEY_A) & 0x8000)
        m_RenderDevice->m_Scene->m_Camera.KeyboardMovement(GraphicsEngine::CameraMovement::LEFT, (float)m_DeltaTime);
    if(GetAsyncKeyState(BANANAS_KEY_D) & 0x8000)
        m_RenderDevice->m_Scene->m_Camera.KeyboardMovement(GraphicsEngine::CameraMovement::RIGHT, (float)m_DeltaTime);
}

void PlatformWin32::CameraMouseCallback(const POINT& pos)
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

    m_RenderDevice->m_Scene->m_Camera.MouseMovement(xoffset, yoffset);
}

}