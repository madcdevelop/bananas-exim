#pragma once

#include "ManagedObject.h"
#include "../../BananasEngine/Core/Core.h"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace BananasEngineDll
{
    public ref class Window : public ManagedObject<GraphicsEngine::Window>
    {
    public:
        Window(IntPtr hInstance, IntPtr hwnd);
        bool Init();
        int Run();

        property IntPtr WindowHandle
        {
        public:
            IntPtr get()
            {
                return (IntPtr)m_Instance->m_hWnd;
            }
        private:
            void set(IntPtr value)
            {
            }
        }        
    };

}
