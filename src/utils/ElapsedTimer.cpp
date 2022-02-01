#include "utils/ElapsedTimer.hpp"

ElapsedTimer::ElapsedTimer() {}

void ElapsedTimer::start() {
    startTime = millis();
}

uint32_t ElapsedTimer::hasElapsedMs() {
    return millis() - startTime;
}