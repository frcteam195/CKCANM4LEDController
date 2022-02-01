#pragma once
#include <Arduino.h>

class ElapsedTimer {
public:
    ElapsedTimer();
    void start();
    uint32_t hasElapsedMs();
private:
    uint32_t startTime;
};