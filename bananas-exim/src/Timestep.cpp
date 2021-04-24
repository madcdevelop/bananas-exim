#include "Timestep.h"

namespace Core
{
    void Timestep::Print(double lastFrameTime, double currentFrameTime, double deltaTime)
    {
        char buffer[256];
        sprintf_s(buffer, "Last Frame Time: %f, Current Frame Time: %f, Delta Time: %f\n", lastFrameTime, currentFrameTime, deltaTime);
        OutputDebugStringA(buffer);
    }

    void Timestep::StartCounter()
    {
        LARGE_INTEGER li;
        if(!QueryPerformanceFrequency(&li))
            OutputDebugStringA("QueryPerformanceFrequency failed!\n");
    
        m_PCFreq = double(li.QuadPart);

        QueryPerformanceCounter(&li);
        m_CounterStart = li.QuadPart;
    }

    /* 
        Get time since timer started in seconds.
    */
    double Timestep::GetTime()
    {
        LARGE_INTEGER li;
        QueryPerformanceCounter(&li);
        return double(li.QuadPart-m_CounterStart)/m_PCFreq;
    }

}