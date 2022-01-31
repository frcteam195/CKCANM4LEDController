#include "drivers/FloatingPixel.hpp"

FloatingPixel::FloatingPixel(uint32_t rgbColor, int stripLength) {
    mRGBColor = rgbColor;
    mStripLength = stripLength;
}

bool FloatingPixel::isAtEnd() {
    return index >= mStripLength || index < 0;
}

void FloatingPixel::increment() {
    index++;
}

void FloatingPixel::decrement() {
    index--;
}

void FloatingPixel::reset() {
    if (index >= mStripLength) {
        index = 0;
    }
    else if (index < 0) {
        index = mStripLength - 1;
    }
}