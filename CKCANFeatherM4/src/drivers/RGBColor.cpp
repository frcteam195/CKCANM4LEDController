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

void HSVtoRGB(float H, float S, float V)
{
    if (H > 360 || H < 0 || S > 100 || S < 0 || V > 100 || V < 0)
    {
        return;
    }
    float s = S / 100;
    float v = V / 100;
    float C = s * v;
    float X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
    float m = v - C;
    float r, g, b;
    if (H >= 0 && H < 60)
    {
        r = C, g = X, b = 0;
    }
    else if (H >= 60 && H < 120)
    {
        r = X, g = C, b = 0;
    }
    else if (H >= 120 && H < 180)
    {
        r = 0, g = C, b = X;
    }
    else if (H >= 180 && H < 240)
    {
        r = 0, g = X, b = C;
    }
    else if (H >= 240 && H < 300)
    {
        r = X, g = 0, b = C;
    }
    else
    {
        r = C, g = 0, b = X;
    }
    int R = (r + m) * 255;
    int G = (g + m) * 255;
    int B = (b + m) * 255;
}