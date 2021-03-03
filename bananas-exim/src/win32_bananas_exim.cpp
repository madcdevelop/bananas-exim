#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include <glad\glad.h>

const int WIDTH  = 800;
const int HEIGHT = 600;

HDC deviceContext;
HGLRC renderContext;

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

    // Window Size
    RECT wr = {0, 0, WIDTH, HEIGHT};
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

    
    // Create Triangle
    static const GLfloat points[] = {
         0.0f,  0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f
    };

    // Vertex Buffer Object
    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);

    // Vertex Array Object
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Run the message loop.
    MSG msg = {0};
    
    bool running = 1;
    while(running)
    {
        // Windows Messages
        while(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            if(msg.message == WM_QUIT)
            {
                running = 0;
            }

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // Update, render
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDisableVertexAttribArray(0);

        SwapBuffers(deviceContext);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
    case WM_CREATE:
        {
            PIXELFORMATDESCRIPTOR pfd = {
                sizeof(PIXELFORMATDESCRIPTOR),  //  size of this pfd  
                1,                     // version number  
                PFD_DRAW_TO_WINDOW |   // support window  
                PFD_SUPPORT_OPENGL |   // support OpenGL  
                PFD_DOUBLEBUFFER,      // double buffered  
                PFD_TYPE_RGBA,         // RGBA type  
                24,                    // 24-bit color depth  
                0, 0, 0, 0, 0, 0,      // color bits ignored  
                0,                     // no alpha buffer  
                0,                     // shift bit ignored  
                0,                     // no accumulation buffer  
                0, 0, 0, 0,            // accum bits ignored  
                32,                    // 32-bit z-buffer      
                0,                     // no stencil buffer  
                0,                     // no auxiliary buffer  
                PFD_MAIN_PLANE,        // main layer  
                0,                     // reserved  
                0, 0, 0                // layer masks ignored  
                };
            deviceContext = GetDC(hwnd);
            int pixelFormat = ChoosePixelFormat(deviceContext, &pfd);
            SetPixelFormat(deviceContext, pixelFormat, &pfd);
            
            // Create Open GL Context
            renderContext = wglCreateContext(deviceContext);
            wglMakeCurrent(deviceContext, renderContext);
            gladLoadGL();
            glViewport(0, 0, WIDTH, HEIGHT);

            MessageBoxA(0, (char*)glGetString(GL_VERSION), "OPENGL VERSION", 0);

            return 0;
        }
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
            // Delete Open GL Context
            ReleaseDC(hwnd, deviceContext);
            wglDeleteContext(renderContext);

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