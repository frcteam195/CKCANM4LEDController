#pragma once
#include <stdint.h>

class FloatingPixel {
public:
	int index = 0;
	int mStripLength;
	uint32_t mRGBColor;

	FloatingPixel(uint32_t rgbColor, int stripLength);
	bool isAtEnd();
	void increment();
	void decrement();
	void reset();
};