#include "Timestep.h"

namespace CoreEngine
{

Timestep::Timestep()
{
}

void Timestep::Print(real32 msPerFrame, real32 fps, real32 megaCyclesPerFrame)
{
    // TODO(neil): Contents may not fit inside buffer or may be a null terminated string. 
    //             Replace in the future.
    char buffer[256];
    sprintf_s(buffer, "%.02fms/f, %.02ff/s, %.02fmc/f\n", msPerFrame, fps, megaCyclesPerFrame);
    OutputDebugStringA(buffer);
}

void Timestep::StartCounter()
{
    LARGE_INTEGER perfCountFrequencyResult;
    if(!QueryPerformanceFrequency(&perfCountFrequencyResult))
        OutputDebugStringA("QueryPerformanceFrequency failed!\n");
    else
        m_PerfCountFrequency = perfCountFrequencyResult.QuadPart;

    m_LastCycleCount = __rdtsc();
    QueryPerformanceCounter(&m_LastCounter);
}

/* 
    Get time since timer started in seconds.
*/
real32 Timestep::GetTime()
{
    m_EndCycleCount = __rdtsc();
    QueryPerformanceCounter(&m_EndCounter);

    m_CyclesElapsed  = m_EndCycleCount - m_LastCycleCount;
    m_CounterElapsed = m_EndCounter.QuadPart - m_LastCounter.QuadPart;

    real32 msPerFrame = ((1000.0f*(real32)m_CounterElapsed) / (real32)m_PerfCountFrequency);
    real32 fps = (real32)m_PerfCountFrequency / (real32)m_CounterElapsed;
    real32 megaCyclesPerFrame = (real32)m_CyclesElapsed / (1000.0f * 1000.0f);

#if 0
    Print(msPerFrame, fps, megaCyclesPerFrame);
#endif

    m_LastCounter = m_EndCounter;
    m_LastCycleCount = m_EndCycleCount;

    return msPerFrame;
}

}