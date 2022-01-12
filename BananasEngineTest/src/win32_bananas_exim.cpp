#ifndef UNICODE
#define UNICODE
#endif

#ifndef ENGINE_TEST_WIN32_BANANAS_EXIM_H
#define ENGINE_TEST_WIN32_BANANAS_EXIM_H

#include "../../BananasEngine/Core/Common.h"
#include "../../BananasEngine/Core/Timestep.h"
#include "../../BananasEngine/Graphics/Renderer.h"

// Windows stuff
HWND            g_windowHandle = NULL;
HINSTANCE       g_hInstance    = NULL;
const wchar_t*  g_windowTitle  = TEXT("Bananas Engine Test");
DWORD           g_windowStyle  = WS_VISIBLE;
bool            g_running      = false;
int32           g_width        = 800;
int32           g_height       = 600;

// Movement
bool  g_firstResize = false;
bool  g_firstMouse = false;
real32 g_lastX = 0.0f;
real32 g_lastY = 0.0f;

// Timestep
real32 g_deltaTime = 0.0f;

// Graphics
GraphicsEngine::Renderer* g_renderer = NULL;
// TODO(neil): replace with interface RenderDevice
GraphicsEngine::RenderDeviceOpenGL* g_renderDevice = NULL;
GraphicsEngine::Scene* g_scene = NULL;

// Core
CoreEngine::Timestep* g_timestep = NULL;

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
    RECT windowRect = {0, 0, (LONG)g_width, (LONG)g_height};
    AdjustWindowRect((LPRECT)&wc, g_windowStyle, FALSE);
    int32 width  = windowRect.right - windowRect.left;
    int32 height = windowRect.bottom - windowRect.top;
    int32 x = GetSystemMetrics(SM_CXSCREEN)/2 - width/2;
    int32 y = GetSystemMetrics(SM_CYSCREEN)/2 - height/2;

    // Create Window
    g_windowStyle |= windowHandle ? WS_CHILD : WS_OVERLAPPEDWINDOW;
    windowHandle = CreateWindow(wc.lpszClassName, g_windowTitle, g_windowStyle, x, y, width, height, windowHandle, NULL, hInstance, NULL);
    if(!windowHandle) MessageBoxA(NULL, "Failed to create window.", "Error", 0);

    // Show Window
    if (windowHandle) {
        SetWindowLongPtr(windowHandle, GWL_STYLE, g_windowStyle);
        ShowWindow(windowHandle, g_windowStyle);
    }

    g_windowHandle = windowHandle;

    // Renderer startup (program startup) init renderer and device
    ProgramStartup();

    g_timestep = new CoreEngine::Timestep;
    g_timestep->StartCounter();
    
    g_running = true;
    while(g_running)
    {
        MSG message;
        while(PeekMessage(&message, NULL, NULL, NULL, PM_REMOVE))
        {
            if(message.message == WM_QUIT)
            {
                g_running = false;
            }

            TranslateMessage(&message);
            DispatchMessage(&message);
        }
        
        if(g_renderDevice)
        {
            if(g_renderDevice->m_running)
            {
                g_renderDevice->Render();
            }
        }

        g_deltaTime = g_timestep->GetTime();
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
                g_firstMouse = true;
            }
        } break;
    case WM_SIZE:
        {
            // skip first call to resize window
            if (g_firstResize) g_renderDevice->Resize();
            else g_firstResize = true;
            
            return 0;
        } break;
    case WM_CLOSE:
        {
            g_running = false;
            return 0;
        } break;
    case WM_DESTROY:
        {
            g_running = false;
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
        g_scene->m_camera.KeyboardMovement(GraphicsEngine::CameraMovement::FORWARD, g_deltaTime);
    if(GetAsyncKeyState(BANANAS_KEY_S) & 0x8000)
        g_scene->m_camera.KeyboardMovement(GraphicsEngine::CameraMovement::BACKWARD, g_deltaTime);
    if(GetAsyncKeyState(BANANAS_KEY_A) & 0x8000)
        g_scene->m_camera.KeyboardMovement(GraphicsEngine::CameraMovement::LEFT, g_deltaTime);
    if(GetAsyncKeyState(BANANAS_KEY_D) & 0x8000)
        g_scene->m_camera.KeyboardMovement(GraphicsEngine::CameraMovement::RIGHT, g_deltaTime);
}

void CameraMouseCallback(const POINT& pos)
{
    if(g_firstMouse)
    {
        g_lastX = (real32)pos.x;
        g_lastY = (real32)pos.y;
        g_firstMouse = false;
    }

    real32 xoffset = (real32)pos.x - g_lastX;
    real32 yoffset = g_lastY - (real32)pos.y;
    g_lastX = (real32)pos.x;
    g_lastY = (real32)pos.y;

    g_scene->m_camera.MouseMovement(xoffset, yoffset);
}

HRESULT ProgramStartup()
{
    // Initialize Renderer
    g_renderer = new GraphicsEngine::Renderer{g_hInstance};
    g_renderer->CreateRenderDevice(g_renderDevice);
    
    // Initialize RenderDevice
    g_renderDevice = g_renderer->m_device;
    g_renderDevice->m_hWnd    = g_windowHandle;
    g_renderDevice->m_width   = g_width;
    g_renderDevice->m_height  = g_height;
    g_renderDevice->m_running = true;
    g_renderDevice->Init();

    // Initialize Scene
    g_scene = new GraphicsEngine::Scene();
    g_renderDevice->m_scene = g_scene;

    // Import and Export
    g_scene->LoadModels("C:\\Code\\bananas-exim\\Content\\Models\\minecraft_hill.obj");
    g_scene->ExportModels("C:\\Code\\bananas-exim\\Content\\Models\\minecraft_hill_export.obj");

    return S_OK;
}

HRESULT ProgramShutdown()
{
    if (g_timestep)
    {
        delete g_timestep;
        g_timestep = NULL;
    }

    if(g_renderDevice)
    {
        g_renderDevice->Shutdown();
    }

    if (g_renderer)
    {
        delete g_renderer;
        g_renderer = NULL;
    }

    return S_OK;
}

#endif
