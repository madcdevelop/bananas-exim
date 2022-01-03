#ifndef CORE_ENGINE_TIMESTEP_H
#define CORE_ENGINE_TIMESTEP_H

#include "Common.h"

namespace CoreEngine
{

class Timestep
{
private:
    int64          m_counterElapsed;
    uint64         m_cyclesElapsed;
    uint64         m_lastCycleCount;
    uint64         m_endCycleCount;
    int64          m_perfCountFrequency;
    LARGE_INTEGER  m_endCounter;
    LARGE_INTEGER  m_lastCounter;

public:
    Timestep();
    ~Timestep();

    void Print(real32 msPerFrame, real32 fps, real32 megaCyclesPerFrame);
    void StartCounter();
    real32 GetTime();
};

}

#endif