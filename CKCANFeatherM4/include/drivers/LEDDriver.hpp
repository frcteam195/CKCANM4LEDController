#pragma once
#include <stdint.h>
#include "drivers/FloatingPixel.hpp"
#include "drivers/RGBColor.hpp"

class LEDDriver
{
public:
	virtual void set(bool on) = 0;
	virtual void setLEDColor(RGBColor rgbColor) = 0;
	virtual void processFade() = 0;
	virtual void processFadeWithSyncPixel(FloatingPixel f, int pixelRateDivisor, bool forward, bool startPixelWhenDim) = 0;
};