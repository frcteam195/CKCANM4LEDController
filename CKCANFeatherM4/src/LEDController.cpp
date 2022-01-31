#include "LEDController.hpp"

LEDController::LEDController(Adafruit_NeoPixel *ledStrip) : mLED(ledStrip)
{
	mIsLEDOn = true;
	setLEDOff();
	mBlinkDurationMs = DEFAULT_BLINK_DURATION_MS;
	mBlinkCount = DEFAULT_BLINK_COUNT;
	mTotalBlinkDurationMs = DEFAULT_TOTAL_BLINK_DURATION_MS;
}

void LEDController::start()
{
	mSystemState = SystemState::OFF;
	setLEDColor(0);
	eTimer.start();
}

void LEDController::setRequestedState(LEDState state)
{
	mRequestedState = state;
}

void LEDController::configureBlink(uint8_t blinkCount, uint16_t blinkDurationMs)
{
	mBlinkCount = blinkCount;
	mBlinkDurationMs = blinkDurationMs;
	mTotalBlinkDurationMs = mBlinkCount * mBlinkDurationMs;
}

void LEDController::setFloatPixel(RGBColor pixelWRGB, uint16_t pixelCount, uint16_t pixelRepeatSpacing)
{

}

void LEDController::setLEDOn()
{
	if (!mIsLEDOn || mSystemState == SystemState::FADING || mSystemState == SystemState::FADING_PIXEL)
	{
		mIsLEDOn = true;
		setLEDColor(mCurrentColor);
	}
}

void LEDController::setLEDOff()
{
	if (mIsLEDOn)
	{
		mIsLEDOn = false;
		setLEDColor(0);
	}
}

void LEDController::setLEDColor(uint32_t wrgb)
{
	mLED->fill(wrgb);
	mLED->show();
}

void LEDController::setLEDColor(RGBColor wrgb)
{
	mLED->fill(wrgb.getUInt32());
	mLED->show();
}

void LEDController::run()
{
	SystemState newState;
	double timeInState = eTimer.hasElapsedMs();
	switch (mSystemState)
	{
	case SystemState::OFF:
	{
		newState = handleOff();
	}
	break;
	case SystemState::BLINKING:
	{
		newState = handleBlinking(timeInState);
	}
	break;
	case SystemState::FIXED_ON:
	{
		newState = handleFixedOn();
	}
	break;
	case SystemState::MORSE:
	{
		newState = handleMorse();
	}
	break;
	case SystemState::FADING:
	{
		newState = handleFade();
	}
	break;
	case SystemState::FADING_PIXEL:
	{
		newState = handleFadePixel();
	}
	break;
	default:
	{
		newState = SystemState::OFF;
	}
	}
	if (newState != mSystemState)
	{
		mSystemState = newState;
		eTimer.start();
	}
}

SystemState LEDController::defaultStateTransfer()
{
	switch (mRequestedState)
	{
	case LEDState::OFF:
		return SystemState::OFF;
	case LEDState::BLINK:
		return SystemState::BLINKING;
	case LEDState::FIXED_ON:
		return SystemState::FIXED_ON;
	case LEDState::MORSE:
		return SystemState::MORSE;
	case LEDState::FADE:
		return SystemState::FADING;
	case LEDState::FADE_PIXEL:
		return SystemState::FADING_PIXEL;
	default:
		return SystemState::OFF;
	}
}

SystemState LEDController::handleOff()
{
	setLEDOff();
	return defaultStateTransfer();
}

SystemState LEDController::handleFixedOn()
{
	setLEDOn();
	return defaultStateTransfer();
}

SystemState LEDController::handleFade()
{
	//Add
	return defaultStateTransfer();
}

SystemState LEDController::handleFadePixel()
{
	//Add
	return defaultStateTransfer();
}

SystemState LEDController::handleBlinking(double timeInState)
{
	//Add
	return defaultStateTransfer();
}

SystemState LEDController::handleMorse()
{
	//Add
	return defaultStateTransfer();
}

SystemState LEDController::returnOffMorse()
{
	//Add
	return defaultStateTransfer();
}
