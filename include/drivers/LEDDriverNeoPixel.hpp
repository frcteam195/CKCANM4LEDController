#pragma once

#include <Adafruit_NeoPixel.h>
#include "drivers/LEDDriver.hpp"
#include "drivers/RGBColor.hpp"

class LEDDriverNeoPixel : public LEDDriver
{
public:
	LEDDriverNeoPixel(Adafruit_NeoPixel& strip);
	void set(bool on) override;
	void setColor(RGBColor rgbColor) override;
	void setBrightness(uint8_t brightness) override;

private:
	Adafruit_NeoPixel* mStrip;
	RGBColor mCurrColor;
	RGBColor mPrevColor;
	uint8_t mCurrBrightness;
	uint8_t mPrevBrightness;
	bool mOn;
};