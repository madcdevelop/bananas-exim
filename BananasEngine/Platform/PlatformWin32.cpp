#include "PlatformWin32.h"

namespace
{
    PlatformEngine::PlatformWin32 *g_PlatformWin32;
}

LRESULT CALLBACK WndProcEngine(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if(g_PlatformWin32)
        return g_PlatformWin32->MsgProc(hwnd, msg, wParam, lParam);
    else
        return DefWindowProc(hwnd, msg, wParam, lParam);
}

namespace PlatformEngine
{

PlatformWin32::PlatformWin32(HINSTANCE hInstance, HWND hwnd)
    : m_hInstance(hInstance), m_hWnd(hwnd), m_hDeviceContext(NULL), m_hRenderContext(NULL), 
      m_width(1280), m_height(720), m_windowTitle(L"Bananas Export/Import"), 
      m_windowStyle(WS_VISIBLE), m_renderer(NULL), m_renderDevice(NULL)
{
    g_PlatformWin32 = this;
    m_timestep = new CoreEngine::Timestep();
    m_timestep->StartCounter();
}

PlatformWin32::~PlatformWin32()
{
    delete m_timestep;
    delete m_renderer;
}

bool PlatformWin32::Win32CreateWindow()
{

#if MEMORY_LEAK_CHECK
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    // Initialize PlatformWin32
    Init();

    // Initialize Renderer
    m_renderer = new GraphicsEngine::Renderer{m_hInstance};
    m_renderer->CreateRenderDevice(m_renderDevice);

    // Initialize RenderDevice
    m_renderDevice = m_renderer->m_device;
    m_renderDevice->m_hWnd    = m_hWnd;
    m_renderDevice->m_width   = m_width;
    m_renderDevice->m_height  = m_height;
    m_renderDevice->m_running = true;
    m_renderDevice->Init();

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
    wc.lpfnWndProc = WndProcEngine;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszClassName = L"WindowClass";
    wc.lpszMenuName = NULL;

    if(!RegisterClass(&wc)) MessageBoxA(NULL, "The window class failed to register.", "Error", 0);

    // Adjust Window Rect for Client Size
    RECT windowRect = {0, 0, (LONG)m_width, (LONG)m_height};
    AdjustWindowRect((LPRECT)&wc, m_windowStyle, FALSE);
    int32 width  = windowRect.right - windowRect.left;
    int32 height = windowRect.bottom - windowRect.top;
    int32 x = GetSystemMetrics(SM_CXSCREEN)/2 - width/2;
    int32 y = GetSystemMetrics(SM_CYSCREEN)/2 - height/2;

    // Create Window
    m_windowStyle |=  m_hWnd ? WS_CHILD : WS_OVERLAPPEDWINDOW;
    m_hWnd = CreateWindow(wc.lpszClassName, m_windowTitle, m_windowStyle, x, y, width, height, m_hWnd, NULL, m_hInstance, NULL);
    if(!m_hWnd) MessageBoxA(NULL, "Failed to create window.", "Error", 0);

    // Show Window
    if (m_hWnd) {
        SetWindowLongPtr(m_hWnd, GWL_STYLE, m_windowStyle);
        ShowWindow(m_hWnd, m_windowStyle);
    }

    return true;
}

int32 PlatformWin32::Run()
{
    MSG message;
    while(PeekMessage(&message, NULL, NULL, NULL, PM_REMOVE))
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
    if(m_renderDevice)
    {
        m_renderDevice->Render();
    }

    m_deltaTime = m_timestep->GetTime();
    
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
                m_firstMouse = true;
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
        m_renderDevice->m_scene->m_camera.KeyboardMovement(GraphicsEngine::CameraMovement::FORWARD, m_deltaTime);
    if(GetAsyncKeyState(BANANAS_KEY_S) & 0x8000)
        m_renderDevice->m_scene->m_camera.KeyboardMovement(GraphicsEngine::CameraMovement::BACKWARD, m_deltaTime);
    if(GetAsyncKeyState(BANANAS_KEY_A) & 0x8000)
        m_renderDevice->m_scene->m_camera.KeyboardMovement(GraphicsEngine::CameraMovement::LEFT, m_deltaTime);
    if(GetAsyncKeyState(BANANAS_KEY_D) & 0x8000)
        m_renderDevice->m_scene->m_camera.KeyboardMovement(GraphicsEngine::CameraMovement::RIGHT, m_deltaTime);
}

void PlatformWin32::CameraMouseCallback(const POINT& pos)
{
    if(m_firstMouse)
    {
        m_lastX = (real32)pos.x;
        m_lastY = (real32)pos.y;
        m_firstMouse = false;
    }

    real32 xoffset = (real32)pos.x - m_lastX;
    real32 yoffset = m_lastY - (real32)pos.y;
    m_lastX = (real32)pos.x;
    m_lastY = (real32)pos.y;

    m_renderDevice->m_scene->m_camera.MouseMovement(xoffset, yoffset);
}

std::string PlatformWin32::GetFilePath(const wchar_t* relativePath)
{
    const uint32 bufsize = 4096;
    TCHAR path[bufsize];
	DWORD result = GetFullPathName(relativePath, bufsize, path, NULL);
    if (result == 0)
    {
        OutputDebugString(L"ERROR\t\tFailed to get full path!\n");
        return "";
    }
    else
    {
        std::wstring wp(&path[0]);
        std::string filePath(wp.begin(), wp.end());
        return filePath;
    }
}

}
