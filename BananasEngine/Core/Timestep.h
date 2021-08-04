#ifndef CORE_ENGINE_TIMESTEP_H
#define CORE_ENGINE_TIMESTEP_H

#include "Common.h"

namespace CoreEngine
{

class Timestep
{
private:
    int64          m_CounterElapsed;
    uint64         m_CyclesElapsed;
    uint64         m_LastCycleCount;
    uint64         m_EndCycleCount;
    int64          m_PerfCountFrequency;
    LARGE_INTEGER  m_EndCounter;
    LARGE_INTEGER  m_LastCounter;

public:
    Timestep();

    void Print(real32 msPerFrame, real32 fps, real32 megaCyclesPerFrame);
    void StartCounter();
    float GetTime();
};

}

#endif