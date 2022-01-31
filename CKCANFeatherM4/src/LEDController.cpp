#include "LEDController.hpp"

LEDController::LEDController(Adafruit_NeoPixel *ledStrip) : mLED(ledStrip)
{
	mIsLEDOn = true;
	setLEDOff();
	mBlinkDuration = DEFAULT_BLINK_DURATION_MS;
	mBlinkCount = DEFAULT_BLINK_COUNT;
	mTotalBlinkDuration = DEFAULT_TOTAL_BLINK_DURATION_MS;
}

void LEDController::start()
{
	mSystemState = SystemState::OFF;
	setLEDColor(0);
	eTimer.start();
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

LEDController::SystemState LEDController::defaultStateTransfer()
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

LEDController::SystemState LEDController::handleOff()
{
	setLEDOff();
	return defaultStateTransfer();
}

LEDController::SystemState LEDController::handleFixedOn()
{
	setLEDOn();
	return defaultStateTransfer();
}

LEDController::SystemState LEDController::handleFade()
{
}

LEDController::SystemState LEDController::handleFadePixel()
{
}

LEDController::SystemState LEDController::handleBlinking(double timeInState)
{
}

LEDController::SystemState LEDController::handleMorse()
{
}

LEDController::SystemState LEDController::returnOffMorse()
{
}
