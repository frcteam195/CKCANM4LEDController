#pragma once

#include <cstdint>
#include <string>
#include <deque>
#include "drivers/LEDDriver.hpp"
#include "utils/TimeoutTimer.hpp"
#include "utils/Colors.hpp"

// Internal state of the system
enum class SystemState
{
    OFF,
    FIXED_ON,
    BLINKING,
    MORSE,
    FADING
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
    FADE
};

class LEDController
{
public:
    LEDController(LEDDriver *ledDriver);
    void init();
    void run();

    void setOff();
    void setOn();
    void setColor(RGBColor wrgb, bool ignoreLock = false);
    void setColor(uint32_t wrgb, bool ignoreLock = false);
    void configBrightness(uint8_t maxBrightness, uint8_t minBrightness);
    void setBrightness(uint8_t brightness);
    void configureBlink(int blinkCount, double blinkDuration);
    void configureDefaultState(LEDState defaultState);
    void configMessage(std::string message, bool autoStartMessage = true);
    void addToMessage(std::string message);
    void setColorLock(bool locked);

    void setRequestedState(LEDState state);
    LEDState getRequestedState();

private:
    SystemState defaultStateTransfer(bool resetColorLock = true);
    SystemState handleOff();
    SystemState handleFixedOn();
    SystemState handleBlinking(uint32_t timeInStateMs);
    SystemState handleMorse();
    SystemState returnOffMorse();
    SystemState handleFading();
    void setDefaultState();


    LEDDriver *mLEDDriver;
    LEDState mDefaultState = LEDState::FIXED_ON;
	SystemState mSystemState = SystemState::OFF;
	LEDState mRequestedState = LEDState::OFF;
	bool mIsLEDOn = false;
    bool mColorLocked = false;
    uint32_t mCurrentStateStartTimeMs = 0;
    uint32_t mBlinkDurationMs = 0;
    uint32_t mBlinkCount = 0;
    uint32_t mTotalBlinkDurationMs = 0;
    TimeoutTimer mTimeoutTimer;
    TimeoutTimer* mBlinkTotalTimer = nullptr;
    TimeoutTimer* mBlinkOnTimer = nullptr;
    uint32_t mDefaultColor = DEFAULT_COLOR;

    int16_t mBrightness = 0;
    uint8_t mMaxBrightness = 255;
    uint8_t mMinBrightness = 50;
    bool fadeDir = false;

    std::deque<std::string> requestedMorseMessage;
    std::deque<std::string>  runningMorseMessage;
    MorseState mMorseState = MorseState::LOAD;
    uint32_t morseStateTimeMs = 0;
    char currentMorseChar = ' ';
    std::deque<char>  currentMorseCode;
    std::string mPrevMessage = "";
    bool mLoopMsg = false;

    static constexpr uint32_t MIN_LED_THREAD_LOOP_MS = 50;
	static constexpr uint32_t DEFAULT_BLINK_COUNT = 6;
	static constexpr uint32_t DEFAULT_BLINK_DURATION_MS = 350;
	static constexpr uint32_t DEFAULT_TOTAL_BLINK_DURATION_MS = DEFAULT_BLINK_COUNT * DEFAULT_BLINK_DURATION_MS;
	static constexpr float SLOW_BLINK_DIVISOR = 1.4;
	static constexpr float FAST_BLINK_DIVISOR = 3.5;
	static constexpr uint32_t LETTER_PAUSE_MS = 500;
	static constexpr uint32_t WORD_PAUSE_MS = 750;

    // static constexpr uint32_t MIN_LED_THREAD_LOOP_MS = 50;
	// static constexpr uint32_t DEFAULT_BLINK_COUNT = 6;
	// static constexpr uint32_t DEFAULT_BLINK_DURATION_MS = 750;
	// static constexpr uint32_t DEFAULT_TOTAL_BLINK_DURATION_MS = DEFAULT_BLINK_COUNT * DEFAULT_BLINK_DURATION_MS;
	// static constexpr float SLOW_BLINK_DIVISOR = 1;
	// static constexpr float FAST_BLINK_DIVISOR = 2;
	// static constexpr uint32_t LETTER_PAUSE_MS = 500;
	// static constexpr uint32_t WORD_PAUSE_MS = 1250;
};