#include "utils/TimeoutTimer.hpp"

#include <cmath>

TimeoutTimer::TimeoutTimer(uint32_t timeoutMs)
{
    this->timeoutMs = timeoutMs;
    firstRun = true;
}

bool TimeoutTimer::isTimedOut()
{
    if (firstRun) {
        eTimer.start();
        firstRun = false;
    }
    return eTimer.hasElapsedMs() > timeoutMs;
}

int32_t TimeoutTimer::timeLeft()
{
    return timeoutMs - eTimer.hasElapsedMs();
}

void TimeoutTimer::reset()
{
    firstRun = true;
}