#pragma once

#include <stdint.h>
#include <cmath>

struct RGBColor
{
    uint8_t white;
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint32_t getUInt32()
    {
        return ((white << 24) & 0xFF000000)
            | ((red << 16) & 0x00FF0000)
            | ((green << 8) & 0x0000FF00)
            | ((blue) & 0x000000FF);
    };
};

struct HSVColor
{
    uint16_t hue;
    uint8_t saturation;
    uint8_t value;
};

HSVColor RGBtoHSV(RGBColor rgbColor);