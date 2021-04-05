#include "Window.h"

namespace CLI
{
    Window::Window(IntPtr hInstance, IntPtr hwnd)
        : ManagedObject(new Core::Window((HINSTANCE)hInstance.ToPointer(), (HWND)hwnd.ToPointer()))
    {
        Console::WriteLine("Creating a new Window-wrapper object!");
    }

    bool Window::Init()
    {
        return m_Instance->Init();
    }

    int Window::Run()
    {
        return m_Instance->Run();
    }
}