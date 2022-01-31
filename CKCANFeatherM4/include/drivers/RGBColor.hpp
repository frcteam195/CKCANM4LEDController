#pragma once

#include <stdint.h>
#include <cmath>

union RGBColor
{
    uint8_t white;
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

union HSVColor
{
    uint16_t hue;
    uint8_t saturation;
    uint8_t value;
};

HSVColor RGBtoHSV(RGBColor rgbColor);