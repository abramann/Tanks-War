#include "Timer.h"

uint32_t CTimer::s_frameTime = 0;

void CTimer::update()
{
    m_delta += s_frameTime;
}

void CTimer::reset()
{
    m_delta = 0;
}

uint32_t CTimer::timeDelta() const
{
    return m_delta;
}
