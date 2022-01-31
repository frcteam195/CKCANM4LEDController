#include "drivers/RGBColor.hpp"

HSVColor RGBtoHSV(RGBColor rgbColor)
{
    HSVColor hsvColor;
    // R, G, B values are divided by 255
    // to change the range from 0..255 to 0..1
    float r1, g1, b1;
    r1 = rgbColor.red / 255.0;
    g1 = rgbColor.green / 255.0;
    b1 = rgbColor.blue / 255.0;

    // h, s, v = hue, saturation, value
    float cmax = std::fmax(r1, std::fmax(g1, b1)); // maximum of r, g, b
    float cmin = std::fmin(r1, std::fmin(g1, b1)); // minimum of r, g, b
    float diff = cmax - cmin;                      // diff of cmax and cmin.
    hsvColor.hue = -1;
    hsvColor.saturation = -1;

    if (cmax == cmin)
    {
        // if cmax and cmax are equal then h = 0
        hsvColor.hue = 0;
    }
    else if (cmax == r1)
    {
        // if cmax equal r then compute h
        hsvColor.hue = (int)((60 * ((g1 - b1) / diff) + 360)) % 360;
    }
    else if (cmax == g1)
    {
        // if cmax equal g then compute h
        hsvColor.hue = (int)((60 * ((b1 - r1) / diff) + 120)) % 360;
    }
    else if (cmax == b1)
    {
        // if cmax equal b then compute h
        hsvColor.hue = (int)((60 * ((r1 - g1) / diff) + 240)) % 360;
    }

    // if cmax equal zero
    if (cmax == 0)
    {
        hsvColor.saturation = 0;
    }
    else
    {
        hsvColor.saturation = (diff / cmax) * 100;
    }

    // compute v
    hsvColor.value = cmax * 100;

    return hsvColor;
}

RGBColor HSVtoRGB(HSVColor hsvColor)
{
    if (hsvColor.hue > 360 || hsvColor.hue < 0 || hsvColor.saturation > 100 || hsvColor.saturation < 0 || hsvColor.value > 100 || hsvColor.value < 0)
    {
        return RGBColor{};
    }
    float s = hsvColor.saturation / 100;
    float v = hsvColor.value / 100;
    float C = s * v;
    float X = C * (1 - abs(fmod(hsvColor.hue / 60.0, 2) - 1));
    float m = v - C;
    float r, g, b;
    if (hsvColor.hue >= 0 && hsvColor.hue < 60)
    {
        r = C, g = X, b = 0;
    }
    else if (hsvColor.hue >= 60 && hsvColor.hue < 120)
    {
        r = X, g = C, b = 0;
    }
    else if (hsvColor.hue >= 120 && hsvColor.hue < 180)
    {
        r = 0, g = C, b = X;
    }
    else if (hsvColor.hue >= 180 && hsvColor.hue < 240)
    {
        r = 0, g = X, b = C;
    }
    else if (hsvColor.hue >= 240 && hsvColor.hue < 300)
    {
        r = X, g = 0, b = C;
    }
    else
    {
        r = C, g = 0, b = X;
    }

    return RGBColor{ 0, (uint8_t)((r + m) * 255), (uint8_t)((g + m) * 255), (uint8_t)((b + m) * 255) };
}