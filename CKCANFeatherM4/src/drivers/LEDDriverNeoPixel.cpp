#include "drivers/LEDDriverNeoPixel.hpp"

LEDDriverNeoPixel::LEDDriverNeoPixel(Adafruit_NeoPixel &strip) : mStrip(&strip)
{
	mOn = false;
	set(false);
}

void LEDDriverNeoPixel::set(bool on)
{
	if (mOn != on || mCurrColor != mPrevColor || mCurrBrightness != mPrevBrightness)
	{
		mStrip->fill(on ? mCurrColor.getUInt32() : 0);
		mStrip->setBrightness(mCurrBrightness);
		mStrip->show();

		mPrevColor = mCurrColor;
		mPrevBrightness = mCurrBrightness;
		mOn = on;
	}
}

void LEDDriverNeoPixel::setColor(RGBColor rgbColor)
{
	mCurrColor = rgbColor;
	set(mOn);
}

void LEDDriverNeoPixel::setBrightness(uint8_t brightness)
{
	mCurrBrightness = brightness;
	set(mOn);
}