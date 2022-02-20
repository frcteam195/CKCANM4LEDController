#include "LEDController.hpp"
#include "utils/MorseCodeTranslator.hpp"
#include "Globals.h"

LEDController::LEDController(LEDDriver* ledDriver) : mLEDDriver(ledDriver), mTimeoutTimer(MIN_LED_THREAD_LOOP_MS) {}

void LEDController::init()
{
    mLEDDriver->setBrightness(255);
	mLEDDriver->setColor(RGBColor{});
	mLEDDriver->set(false);

	mIsLEDOn = true;
	setOff();

	setColor(mDefaultColor);
	mBlinkDurationMs = DEFAULT_BLINK_DURATION_MS;
	mBlinkCount = DEFAULT_BLINK_COUNT;
	mTotalBlinkDurationMs = DEFAULT_TOTAL_BLINK_DURATION_MS;

	mSystemState = SystemState::OFF;
	mCurrentStateStartTimeMs = millis();
	mTimeoutTimer.reset();

#ifdef DEBUG
	Serial.println("LEDController init completed!");
#endif

}

void LEDController::run()
{
#ifdef DEBUG
	// Serial.println("Entering LEDController run...");
	// Serial.printf("Time left: %d\n", mTimeoutTimer.timeLeft());
#endif
	if (mTimeoutTimer.isTimedOut())
	{
#ifdef DEBUG
		// Serial.println("LEDController loop time to run...");
#endif
		SystemState newState = SystemState::OFF;
		uint32_t timeInStateMs = millis() - mCurrentStateStartTimeMs;
		switch (mSystemState) {
			case SystemState::OFF:
			{
				newState = handleOff();
				break;
			}
			case SystemState::BLINKING:
			{
				newState = handleBlinking(timeInStateMs);
				break;
			}
			case SystemState::FIXED_ON:
			{
				newState = handleFixedOn();
				break;
			}
			case SystemState::MORSE:
			{
				newState = handleMorse();
				break;
			}
			case SystemState::FADING:
			{
				newState = handleFading();
				break;
			}
		}

#ifdef DEBUG
		// Serial.printf("Curr State: %d\n", mSystemState);
		// Serial.printf("New State: %d\n", newState);
#endif

		if (newState != mSystemState) {
			mSystemState = newState;
			mCurrentStateStartTimeMs = millis();
		}

		mLEDDriver->set(mIsLEDOn);

		mTimeoutTimer.reset();
	}
}

SystemState LEDController::defaultStateTransfer(bool resetColorLock)
{
	if (resetColorLock)
	{
		setColorLock(false);
	}

	switch (mRequestedState) {
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
		default:
			return SystemState::OFF;
	}
}

SystemState LEDController::handleOff()
{
	setOff();
	return defaultStateTransfer();
}

SystemState LEDController::handleFixedOn()
{
	setOn();
	return defaultStateTransfer();
}

SystemState LEDController::handleFading()
{
	if (fadeDir)
	{
		if (mBrightness < mMaxBrightness)
		{
			mBrightness += 7;
		}
		else if (mBrightness >= mMaxBrightness)
		{
			fadeDir = false;
		}
	}
	else
	{
		if (mBrightness > mMinBrightness)
		{
			mBrightness -= 7;
		}
		else if (mBrightness <= mMinBrightness)
		{
			fadeDir = true;
		}
	}

	mBrightness = std::max((int16_t)mMinBrightness, std::min(mBrightness, (int16_t)mMaxBrightness));

#ifdef DEBUG
	Serial.printf("Brightness: %d\n", mBrightness);
#endif

	mLEDDriver->setBrightness((uint8_t)mBrightness);
	setOn();

	return defaultStateTransfer();
}

SystemState LEDController::handleBlinking(uint32_t timeInStateMs)
{
	if (timeInStateMs > mTotalBlinkDurationMs) {
		//setLEDOff();
		// Transition to OFF state and clear wanted state.
		//setRequestedState(LEDState.OFF);
		setDefaultState();
		setOff();
		// setColor(mDefaultColor);
		return SystemState::OFF;
	}

	uint32_t cycleNum = (uint32_t) (timeInStateMs / (mBlinkDurationMs));
	if ((cycleNum % 2) == 0) {
		setOn();
	} else {
		setOff();
	}
	return SystemState::BLINKING;
}

SystemState LEDController::handleMorse()
{
	switch (mMorseState) {
		case MorseState::LOAD:
		{
			if (requestedMorseMessage.size() <= 0)
			{
				return returnOffMorse();
			}
			runningMorseMessage = std::deque<std::string>(requestedMorseMessage);
			if (runningMorseMessage.size() > 0)
			{
				setOff();
				mMorseState = MorseState::NEXT_CHAR;
			} else {
				return returnOffMorse();
			}
			break;
		}
		case MorseState::NEXT_CHAR:
		{
#ifdef DEBUG
			Serial.printf("Curr Morse Message Size: %d\n", runningMorseMessage.size());
#endif 
			if (runningMorseMessage.size() > 0) {
				std::string s = runningMorseMessage.front();
				runningMorseMessage.pop_front();
				currentMorseCode = std::deque<char>(s.begin(), s.end());
				mMorseState = MorseState::NEXT_MORSE_CODE;
			} else {
				mMorseState = MorseState::DONE;
				morseStateTimeMs = millis();
			}
			break;
		}
		case MorseState::NEXT_MORSE_CODE:
		{
			if (currentMorseCode.size() > 0)
			{
				currentMorseChar = currentMorseCode.front();
				currentMorseCode.pop_front();
				mMorseState = MorseState::BLINK_ON;
				morseStateTimeMs = millis();
			}
			else {
				//Delay to delineate between letters in a string
				if ((millis() - morseStateTimeMs) > LETTER_PAUSE_MS) {
					mMorseState = MorseState::NEXT_CHAR;
					//morseStateTime = Timer.getFPGATimestamp();
				}
			}
			break;
		}
		case MorseState::BLINK_ON:
		{
			if (currentMorseChar == '.' || currentMorseChar == '-') {
				double divisorVal = currentMorseChar == '.' ? FAST_BLINK_DIVISOR : SLOW_BLINK_DIVISOR;
				setOn();
				if ((millis() - morseStateTimeMs) > mBlinkDurationMs / divisorVal) {
					mMorseState = MorseState::BLINK_OFF;
					morseStateTimeMs = millis();
				}
			} else {
				mMorseState = MorseState::NEXT_MORSE_CODE;
			}
			break;
		}
		case MorseState::BLINK_OFF:
		{
			if (currentMorseChar == '.' || currentMorseChar == '-') {
				float divisorVal = currentMorseChar == '.' ? FAST_BLINK_DIVISOR : SLOW_BLINK_DIVISOR;
				setOff();
				if ((millis() - morseStateTimeMs) > mBlinkDurationMs / divisorVal) {
					mMorseState = MorseState::NEXT_MORSE_CODE;
					currentMorseChar = ' ';
					morseStateTimeMs = millis();
				}
			} else {
				mMorseState = MorseState::NEXT_MORSE_CODE;
			}
			break;
		}
		case MorseState::DONE:
		{
			if ((millis() - morseStateTimeMs) > WORD_PAUSE_MS) {
				return returnOffMorse();
			}
			break;
		}
		default:
			return returnOffMorse();
	}

	return SystemState::MORSE;
}

SystemState LEDController::returnOffMorse()
{
	mMorseState = MorseState::LOAD;
	morseStateTimeMs = 0;
	currentMorseChar = ' ';

#ifdef DEBUG
	Serial.printf("Loop state: %d\n", (int)mLoopMsg);
#endif

	// if (mDefaultState == LEDState::MORSE)
	// {
	// 	setDefaultState();
	// }

	return defaultStateTransfer(false);
	
	// setOff();
	// return SystemState::OFF;
}

void LEDController::setDefaultState()
{
	setRequestedState(mDefaultState);
	// setColor(mDefaultColor);
}

void LEDController::setRequestedState(LEDState state)
{
	mRequestedState = state;
#ifdef DEBUG
	Serial.printf("Setting state to %d\n", mRequestedState);
#endif
}

LEDState LEDController::getRequestedState()
{
	return mRequestedState;
}

void LEDController::setOff()
{
	if (mIsLEDOn) {
		mIsLEDOn = false;
		mLEDDriver->set(false);
	}
}

void LEDController::setOn()
{
	if (!mIsLEDOn) {
		mIsLEDOn = true;
		mLEDDriver->set(true);
	}
#ifdef DEBUG

#endif
}

void LEDController::setColorLock(bool locked)
{
	mColorLocked = locked;
}

void LEDController::setColor(RGBColor wrgb, bool ignoreLock)
{
	if (!mColorLocked || ignoreLock)
	{
		mLEDDriver->setColor(wrgb);
	}
}

void LEDController::setColor(uint32_t wrgb, bool ignoreLock)
{
	if (!mColorLocked || ignoreLock)
	{
		mLEDDriver->setColor(RGBColor::fromUInt32(wrgb));
	}
}

void LEDController::configBrightness(uint8_t maxBrightness, uint8_t minBrightness)
{
	mMaxBrightness = maxBrightness;
	mMinBrightness = minBrightness;
}

void LEDController::setBrightness(uint8_t brightness)
{
	mBrightness = brightness;
	mLEDDriver->setBrightness(brightness);
}

void LEDController::configureDefaultState(LEDState defaultState)
{
	mDefaultState = defaultState;
}

void LEDController::configureBlink(int blinkCount, double blinkDurationMs)
{
	mBlinkDurationMs = blinkDurationMs;
	mBlinkCount = blinkCount;
	mTotalBlinkDurationMs = mBlinkCount * mBlinkDurationMs;
}

void LEDController::configMessage(std::string message, bool autoStartMessage)
{
	if (message != mPrevMessage) {
		MorseCodeTranslator::getTranslatedMsg(message, requestedMorseMessage);
		mPrevMessage = message;
	}

	if (autoStartMessage)
	{
		setRequestedState(LEDState::MORSE);
	}
}

void LEDController::addToMessage(std::string message)
{
	std::deque<std::string> tmp;
	MorseCodeTranslator::getTranslatedMsg(message, tmp);
	requestedMorseMessage.insert(requestedMorseMessage.end(), tmp.begin(), tmp.end());
}