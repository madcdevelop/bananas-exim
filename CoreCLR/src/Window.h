#pragma once

#include "ManagedObject.h"
#include "../../bananas-exim/src/Core.h"

using namespace System;

namespace CLI
{
    public ref class Window : public ManagedObject<Core::Window>
    {
    public:
        Window(IntPtr hInstance);
    };

}
