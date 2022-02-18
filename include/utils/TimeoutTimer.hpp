#pragma once

#include "ElapsedTimer.hpp"

class TimeoutTimer {
public:
    TimeoutTimer(uint32_t timeoutMs);
    bool isTimedOut();
    void reset();
    int32_t timeLeft();

private:
    uint32_t timeoutMs = 0;
    bool firstRun = true;
    ElapsedTimer eTimer;
};