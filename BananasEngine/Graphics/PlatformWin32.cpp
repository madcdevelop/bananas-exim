#include "PlatformWin32.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
    case WM_KEYDOWN:
        {
        } break;
    case WM_MOUSEMOVE:
        {
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

namespace GraphicsEngine
{

PlatformWin32::PlatformWin32(HINSTANCE hInstance, HWND hwnd)
    : m_hInstance(hInstance), m_hWnd(hwnd), m_hDeviceContext(NULL), m_hRenderContext(NULL), 
      m_Width(800), m_Height(600), m_WindowTitle(L"Bananas Export/Import"), 
      m_WindowStyle(WS_VISIBLE)
{
}

PlatformWin32::~PlatformWin32()
{
}

bool PlatformWin32::Win32CreateWindow()
{
    // Init PlatformWin32
    Init();
    // Init RenderDevice
    

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

}