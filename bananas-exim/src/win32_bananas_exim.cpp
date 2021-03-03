#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <glad\glad.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

// Entry point to program
int WINAPI 
wWinMain(HINSTANCE hInstance,
         HINSTANCE hPrevInstance,
         LPWSTR lpCmdLine,
         int nShowCmd)
{
    // Register the window class.
    WNDCLASS wc = {0};
    
    const wchar_t CLASS_NAME[]  = L"Main Windows Class";
    wc.lpszClassName = CLASS_NAME;
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;

    RegisterClass(&wc);

    // Create the window.
    HWND hwnd;

    int width  = 640;
    int height = 480;
    RECT wr = {0, 0, width, height};
    AdjustWindowRect((LPRECT)&wc, WS_OVERLAPPEDWINDOW, FALSE);

    hwnd = CreateWindow(CLASS_NAME, L"Bananas Export/Import Tool",
                        WS_OVERLAPPEDWINDOW|WS_VISIBLE, 
                        0, 0, 
                        wr.right - wr.left,
                        wr.bottom - wr.top,
                        0, 0, hInstance, 0);

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nShowCmd);

    // Run the message loop.
    MSG msg = {0};

    int running = 1;

    while(running)
    {
        while(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            if(msg.message == WM_QUIT)
            {
                running = 0;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
    case WM_CLOSE:
        {
            if(MessageBox(hwnd, L"Quit?", L"App", MB_OKCANCEL) == IDOK)
            {
                DestroyWindow(hwnd);
            }
            return 0;
        } break;
    case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        } break;
    default:
        {
            return DefWindowProc(hwnd, message, wParam, lParam);
        } break;
    }
    return 0;
}