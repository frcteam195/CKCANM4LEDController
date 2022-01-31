#pragma once

#include <Adafruit_NeoPixel.h>
#include "drivers/LEDDriver.hpp"
#include "utils/Colors.hpp"
#include "drivers/RGBColor.hpp"

class LEDDriverNeoPixel : public LEDDriver
{
public:
	LEDDriverNeoPixel(Adafruit_NeoPixel& strip);
	void set(bool on) override;
	void setLEDColor(RGBColor rgbColor) override;
	void processFade() override;
	void processFadeWithSyncPixel(FloatingPixel f, int pixelRateDivisor, bool forward, bool startPixelWhenDim) override;

private:
	Adafruit_NeoPixel* mStrip;
	RGBColor mCurrColor;
	RGBColor mPrevColor;
	float mPrevInc = 0;
	float mCurrColorHSV[3];
	int mCurrColorRGB = 0;
	bool mOn;
	static constexpr float kMinBrightness = 0.35f;
	static constexpr float kMaxBrightness = 0.9f;
	static constexpr float kFadeIncrement = 0.032f;
};