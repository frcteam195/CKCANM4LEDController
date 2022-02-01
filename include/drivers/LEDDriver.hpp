#pragma once
#include <stdint.h>
#include "drivers/RGBColor.hpp"

class LEDDriver
{
public:
	virtual void set(bool on) = 0;
	virtual void setColor(RGBColor rgbColor) = 0;
	virtual void setBrightness(uint8_t brightness) = 0;
};