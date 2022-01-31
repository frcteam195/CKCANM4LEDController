#pragma once

#include <Adafruit_NeoPixel.h>
#include "drivers/LEDDriver.hpp"
#include "utils/Colors.hpp"

class LEDDriverNeoPixel : public LEDDriver
{
public:
	LEDDriverNeoPixel(Adafruit_NeoPixel& strip);
	void set(bool on) override;
	void setLEDColor(uint32_t rgbColor) override;
	void processFade() override;
	void processFadeWithSyncPixel(FloatingPixel f, int pixelRateDivisor, bool forward, bool startPixelWhenDim) override;

private:
	Adafruit_NeoPixel* mStrip;
	uint32_t mCurrColor;
	uint32_t mPrevColor;
	float mPrevInc = 0;
	float mCurrColorHSV[3];
	int mCurrColorRGB = 0;
	bool mOn;
	static constexpr float kMinBrightness = 0.35f;
	static constexpr float kMaxBrightness = 0.9f;
	static constexpr float kFadeIncrement = 0.032f;
};