#include "drivers/LEDDriverNeoPixel.hpp"

LEDDriverNeoPixel::LEDDriverNeoPixel(Adafruit_NeoPixel &strip) : mStrip(&strip)
{
	mOn = false;
	set(false);
}

void LEDDriverNeoPixel::set(bool on)
{
	if (mOn != on || mCurrColor != mPrevColor)
	{
		if (on)
		{
			mStrip->fill(mCurrColor.getUInt32());
			mStrip->show();
		}
		else
		{
			mStrip->fill(0);
			mStrip->show();
		}

		mPrevColor = mCurrColor;
		mOn = on;
	}
}

void LEDDriverNeoPixel::setLEDColor(RGBColor rgbColor)
{
	mCurrColor = rgbColor;
	set(mOn);
}

void LEDDriverNeoPixel::processFade()
{
}

void LEDDriverNeoPixel::processFadeWithSyncPixel(FloatingPixel f, int pixelRateDivisor, bool forward, bool startPixelWhenDim)
{
}