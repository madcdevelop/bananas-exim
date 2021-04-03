#include "Window.h"

namespace CLI
{
    Window::Window(IntPtr hInstance)
        : ManagedObject(new Core::Window((HINSTANCE)hInstance.ToPointer()))
    {
        Console::WriteLine("Creating a new Window-wrapper object!");
    }
}