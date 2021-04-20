#pragma once

#include "Common.h"

namespace Core
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