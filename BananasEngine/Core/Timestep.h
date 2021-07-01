#ifndef CORE_ENGINE_TIMESTEP_H
#define CORE_ENGINE_TIMESTEP_H

#include "Common.h"

namespace CoreEngine
{

class Timestep
{
public:
    Timestep()
        :m_PCFreq(0.0), m_CounterStart(0)
    {
    }

private:
    double m_PCFreq;
    long long m_CounterStart;

public:
    void Print(double lastFrameTime, double currentFrameTime, double deltaTime);

    void StartCounter();
    double GetTime();

};

}

#endif