#ifndef UNICODE
#define UNICODE
#endif

#include "Common.h"
#include "Renderer.h"
#include "Timestep.h"

// Windows stuff
HWND            g_windowHandle = NULL;
HINSTANCE       g_hInstance    = NULL;
const wchar_t*  g_WindowTitle  = TEXT("Bananas Engine Test");
DWORD           g_WindowStyle  = WS_VISIBLE;
bool            g_Running      = false;
int32           g_Width        = 800;
int32           g_Height       = 600;

// Movement
bool  g_FirstResize = false;
bool  g_FirstMouse = false;
float g_LastX = 0.0f;
float g_LastY = 0.0f;

// Timestep
double g_DeltaTime = 0.0;
double g_LastFrameTime = 0.0;

// Graphics
GraphicsEngine::Renderer* g_Renderer = NULL;
// TODO(neil): replace with interface RenderDevice
GraphicsEngine::RenderDeviceOpenGL* g_RenderDevice = NULL;
GraphicsEngine::Scene* g_Scene = NULL;

// Core
CoreEngine::Timestep* g_Timestep = NULL;

// Functions
LRESULT CALLBACK WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);
void CameraKeyboardCallback();
void CameraMouseCallback(const POINT& pos);
HRESULT ProgramStartup();
HRESULT ProgramShutdown();

// Entry point to program
int32 APIENTRY 
WinMain(HINSTANCE hInstance,
        HINSTANCE hPrevInstance,
        LPSTR     lpCmdLine,
        int32       nShowCmd)
{
    HWND windowHandle = NULL;
    
    // initialize the window
    WNDCLASS wc;
    ZeroMemory(&wc, sizeof(WNDCLASS));
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.hInstance = hInstance;
    wc.lpfnWndProc = WndProc;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.lpszClassName = L"WindowClass";
    wc.lpszMenuName = NULL;

    if(!RegisterClass(&wc)) MessageBoxA(NULL, "The window class failed to register.", "Error", 0);

    // Adjust Window Rect for Client Size
    RECT windowRect = {0, 0, (LONG)g_Width, (LONG)g_Height};
    AdjustWindowRect((LPRECT)&wc, g_WindowStyle, FALSE);
    int32 width  = windowRect.right - windowRect.left;
    int32 height = windowRect.bottom - windowRect.top;
    int32 x = GetSystemMetrics(SM_CXSCREEN)/2 - width/2;
    int32 y = GetSystemMetrics(SM_CYSCREEN)/2 - height/2;

    // Create Window
    g_WindowStyle |= windowHandle ? WS_CHILD : WS_OVERLAPPEDWINDOW;
    windowHandle = CreateWindow(wc.lpszClassName, g_WindowTitle, g_WindowStyle, x, y, width, height, windowHandle, NULL, hInstance, NULL);
    if(!windowHandle) MessageBoxA(NULL, "Failed to create window.", "Error", 0);

    // Show Window
    if (windowHandle) {
        SetWindowLongPtr(windowHandle, GWL_STYLE, g_WindowStyle);
        ShowWindow(windowHandle, g_WindowStyle);
    }

    g_windowHandle = windowHandle;

    // Renderer startup (program startup) init renderer and device
    ProgramStartup();

    g_Timestep = new CoreEngine::Timestep;
    g_Timestep->StartCounter();
    
    g_Running = true;
    while(g_Running)
    {
        double time = g_Timestep->GetTime();
        g_DeltaTime = time - g_LastFrameTime;
        g_LastFrameTime = time;

        MSG message;
        while(PeekMessage(&message, NULL, NULL, NULL, PM_REMOVE))
        {
            if(message.message == WM_QUIT)
            {
                g_Running = false;
            }

            TranslateMessage(&message);
            DispatchMessage(&message);
        }
        
        if(g_RenderDevice)
        {
            if(g_RenderDevice->m_Running)
            {
                g_RenderDevice->Render();
            }
        }
    }

    ProgramShutdown();

    return 0;
}

LRESULT CALLBACK WndProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
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
                g_FirstMouse = true;
            }
        } break;
    case WM_SIZE:
        {
            // skip first call to resize window
            if (g_FirstResize) g_RenderDevice->Resize();
            else g_FirstResize = true;
            
            return 0;
        } break;
    case WM_CLOSE:
        {
            g_Running = false;
            return 0;
        } break;
    case WM_DESTROY:
        {
            g_Running = false;
            PostQuitMessage(0);
            return 1;
        } break;
    default:
        return DefWindowProc(windowHandle, message, wParam, lParam);
    }
    return 0;
}

void CameraKeyboardCallback()
{
    if(GetAsyncKeyState(BANANAS_KEY_W) & 0x8000)
        g_Scene->m_Camera.KeyboardMovement(GraphicsEngine::CameraMovement::FORWARD, (float)g_DeltaTime);
    if(GetAsyncKeyState(BANANAS_KEY_S) & 0x8000)
        g_Scene->m_Camera.KeyboardMovement(GraphicsEngine::CameraMovement::BACKWARD, (float)g_DeltaTime);
    if(GetAsyncKeyState(BANANAS_KEY_A) & 0x8000)
        g_Scene->m_Camera.KeyboardMovement(GraphicsEngine::CameraMovement::LEFT, (float)g_DeltaTime);
    if(GetAsyncKeyState(BANANAS_KEY_D) & 0x8000)
        g_Scene->m_Camera.KeyboardMovement(GraphicsEngine::CameraMovement::RIGHT, (float)g_DeltaTime);
}

void CameraMouseCallback(const POINT& pos)
{
    if(g_FirstMouse)
    {
        g_LastX = (float)pos.x;
        g_LastY = (float)pos.y;
        g_FirstMouse = false;
    }

    float xoffset = (float)pos.x - g_LastX;
    float yoffset = g_LastY - (float)pos.y;
    g_LastX = (float)pos.x;
    g_LastY = (float)pos.y;

    g_Scene->m_Camera.MouseMovement(xoffset, yoffset);
}

HRESULT ProgramStartup()
{
    // Initialize Renderer
    g_Renderer = new GraphicsEngine::Renderer{g_hInstance};
    g_Renderer->CreateRenderDevice(g_RenderDevice);
    
    // Initialize RenderDevice
    g_RenderDevice = g_Renderer->m_Device;
    g_RenderDevice->m_hWnd    = g_windowHandle;
    g_RenderDevice->m_Width   = g_Width;
    g_RenderDevice->m_Height  = g_Height;
    g_RenderDevice->m_Running = true;
    g_RenderDevice->Init();

    // Initialize Scene
    g_Scene = new GraphicsEngine::Scene();
    g_RenderDevice->m_Scene = g_Scene;

    // Import and Export
    g_Scene->LoadModels("C:\\Code\\bananas-exim\\Content\\Models\\minecraft_hill.obj");
    g_Scene->ExportModels("C:\\Code\\bananas-exim\\Content\\Models\\minecraft_hill_export.obj");

    return S_OK;
}

HRESULT ProgramShutdown()
{
    if (g_Timestep)
    {
        delete g_Timestep;
        g_Timestep = NULL;
    }

    if(g_RenderDevice)
    {
        g_RenderDevice->Shutdown();
    }

    if (g_Renderer)
    {
        delete g_Renderer;
        g_Renderer = NULL;
    }

    return S_OK;
}
