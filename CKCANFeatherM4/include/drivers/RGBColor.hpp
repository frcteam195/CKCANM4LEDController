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

    static RGBColor fromUInt32(uint32_t wrgb)
    {
        return RGBColor{(uint8_t)((wrgb >> 24) & 0xFF), (uint8_t)((wrgb >> 16) & 0xFF), (uint8_t)((wrgb >> 8) & 0xFF), (uint8_t)((wrgb) & 0xFF)};
    }

    bool operator==(const RGBColor& o) const
    {
        return (white == o.white
            && red == o.red
            && green == o.green
            && blue == o.blue);
    };
    bool operator!=(const RGBColor& o) const { return !((*this) == o); };
};

struct HSVColor
{
    uint16_t hue;
    uint8_t saturation;
    uint8_t value;

    bool operator==(const HSVColor& o) const
    {
        return (hue == o.hue
            && saturation == o.saturation
            && value == o.value);
    };
    bool operator!=(const HSVColor& o) const { return !((*this) == o); };
};

HSVColor RGBtoHSV(RGBColor rgbColor);
RGBColor HSVtoRGB(HSVColor hsvColor);