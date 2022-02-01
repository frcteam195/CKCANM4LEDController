#pragma once

#include <cstdint>
#include <string>
#include <deque>
#include "drivers/LEDDriver.hpp"
#include "utils/TimeoutTimer.hpp"

// Internal state of the system
enum class SystemState
{
    OFF,
    FIXED_ON,
    BLINKING,
    MORSE
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
    MORSE
};

class LEDController
{
public:
    LEDController(LEDDriver *ledDriver);
    void init();
    void run();

    void setOff();
    void setOn();
    void setColor(RGBColor wrgb);
    void setColor(uint32_t wrgb);
    void setBrightness(uint8_t brightness);
    void configureBlink(int blinkCount, double blinkDuration);
    void configureDefaultState(LEDState defaultState);
    void configMessage(std::string message, bool autoStartMessage = true);
    void addToMessage(std::string message);

    void setRequestedState(LEDState state);

private:
    SystemState defaultStateTransfer();
    SystemState handleOff();
    SystemState handleFixedOn();
    SystemState handleBlinking(uint32_t timeInState);
    SystemState handleMorse();
    SystemState returnOffMorse();
    void setDefaultState();


    LEDDriver *mLEDDriver;
    LEDState mDefaultState = LEDState::FIXED_ON;
	SystemState mSystemState = SystemState::OFF;
	LEDState mRequestedState = LEDState::OFF;
	bool mIsLEDOn;
    uint32_t mCurrentStateStartTimeMs;
    uint32_t mBlinkDurationMs;
    uint32_t mBlinkCount;
    uint32_t mTotalBlinkDurationMs;
    TimeoutTimer mTimeoutTimer;

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
	static constexpr uint32_t DEFAULT_BLINK_DURATION_MS = 200;
	static constexpr uint32_t DEFAULT_TOTAL_BLINK_DURATION_MS = DEFAULT_BLINK_COUNT * DEFAULT_BLINK_DURATION_MS;
	static constexpr float SLOW_BLINK_DIVISOR = 1.5;
	static constexpr float FAST_BLINK_DIVISOR = 3;
	static constexpr uint32_t LETTER_PAUSE_MS = 500;
	static constexpr uint32_t WORD_PAUSE_MS = 750;
};