#pragma once
#include <stdint.h>
#include "RGBColor.hpp"

class FloatingPixel {
public:
	int index = 0;
	int mStripLength;
	RGBColor mRGBColor;

	FloatingPixel(RGBColor rgbColor, int stripLength);
	bool isAtEnd();
	void increment();
	void decrement();
	void reset();
};