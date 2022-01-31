#pragma once
#include <Adafruit_NeoPixel.h>
#include "utils/ElapsedTimer.hpp"
#include "utils/Colors.hpp"
#include "drivers/RGBColor.hpp"

enum class SystemState
{
	OFF,
	FIXED_ON,
	BLINKING,
	MORSE,
	FADING,
	FADING_PIXEL
};

enum class MorseState
{
	LOAD,
	BLINK_ON,
	BLINK_OFF,
	NEXT_CHAR,
	NEXT_MORSE_CODE,
	DONE
};

enum class LEDState
{
	OFF,
	FIXED_ON,
	BLINK,
	MORSE,
	FADE,
	FADE_PIXEL
};

class LEDController
{
public:
	LEDController(Adafruit_NeoPixel *ledStrip);
	void start();
	void run();

	void setLEDColor(RGBColor wrgb);
	void setLEDColor(uint32_t wrgb);
	void setLEDOn();
	void setLEDOff();

	void setRequestedState(LEDState state);
	void setLEDDefaultState();
	void setFloatPixel(RGBColor pixelWRGB, uint16_t pixelCount, uint16_t pixelRepeatSpacing);

	void configureBlink(uint8_t blinkCount, uint16_t blinkDurationMs);
	void setLEDDefaultState(LEDState defaultState);

private:
	SystemState defaultStateTransfer();
	SystemState handleOff();
	SystemState handleFixedOn();
	SystemState handleFade();
	SystemState handleFadePixel();
	SystemState handleBlinking(double timeInState);
	SystemState handleMorse();
	SystemState returnOffMorse();

	static constexpr int MIN_LED_THREAD_LOOP_MS = 50;
	static constexpr int DEFAULT_BLINK_COUNT = 6;
	static constexpr int DEFAULT_BLINK_DURATION_MS = 200;
	static constexpr int DEFAULT_TOTAL_BLINK_DURATION_MS = DEFAULT_BLINK_COUNT * DEFAULT_BLINK_DURATION_MS;
	static constexpr int SLOW_BLINK_DIVISOR = 1.5;
	static constexpr int FAST_BLINK_DIVISOR = 3;
	static constexpr int LETTER_PAUSE_MS = 500;
	static constexpr int WORD_PAUSE_MS = 750;

	LEDState mDefaultState = LEDState::FADE;
	SystemState mSystemState = SystemState::OFF;
	LEDState mRequestedState = LEDState::BLINK;
	bool mIsLEDOn;
	Adafruit_NeoPixel *mLED;
	uint32_t mCurrentColor = DEFAULT_COLOR;
	float mCurrentStateStartTime;
	float mBlinkDurationMs;
	int mBlinkCount;
	float mTotalBlinkDurationMs;
	ElapsedTimer eTimer;
};