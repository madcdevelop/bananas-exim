#pragma once

#include "ManagedObject.h"
#include "../../bananas-exim/src/Core.h"

using namespace System;
using namespace System::Runtime::InteropServices;

namespace CLI
{
    public ref class Window : public ManagedObject<Core::Window>
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
