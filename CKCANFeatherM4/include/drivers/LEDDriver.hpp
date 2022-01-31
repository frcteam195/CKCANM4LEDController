#pragma once
#include <stdint.h>
#include "drivers/FloatingPixel.hpp"

class LEDDriver
{
public:
	virtual void set(bool on) = 0;
	virtual void setLEDColor(uint32_t rgbColor) = 0;
	virtual void processFade() = 0;
	virtual void processFadeWithSyncPixel(FloatingPixel f, int pixelRateDivisor, bool forward, bool startPixelWhenDim) = 0;
};