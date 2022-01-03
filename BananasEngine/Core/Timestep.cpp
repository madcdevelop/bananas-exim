#include "Timestep.h"

namespace CoreEngine
{

Timestep::Timestep()
{
}

Timestep::~Timestep()
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
        m_perfCountFrequency = perfCountFrequencyResult.QuadPart;

    m_lastCycleCount = __rdtsc();
    QueryPerformanceCounter(&m_lastCounter);
}

/* 
    Get time since timer started in seconds.
*/
real32 Timestep::GetTime()
{
    m_endCycleCount = __rdtsc();
    QueryPerformanceCounter(&m_endCounter);

    m_cyclesElapsed  = m_endCycleCount - m_lastCycleCount;
    m_counterElapsed = m_endCounter.QuadPart - m_lastCounter.QuadPart;

    real32 msPerFrame = ((1000.0f*(real32)m_counterElapsed) / (real32)m_perfCountFrequency);
    real32 fps = (real32)m_perfCountFrequency / (real32)m_counterElapsed;
    real32 megaCyclesPerFrame = (real32)m_cyclesElapsed / (1000.0f * 1000.0f);
#if 0
    Print(msPerFrame, fps, megaCyclesPerFrame);
#endif

    m_lastCounter = m_endCounter;
    m_lastCycleCount = m_endCycleCount;

    return msPerFrame;
}

}