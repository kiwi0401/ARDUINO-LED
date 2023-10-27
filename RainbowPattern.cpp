#include "RainbowPattern.h"


int Wrap(int kX, int const kLowerBound, int const kUpperBound)
{
    int range_size = kUpperBound - kLowerBound + 1;

    if (kX < kLowerBound)
        kX += range_size * ((kLowerBound - kX) / range_size + 1);

    return kLowerBound + (kX - kLowerBound) % range_size;
}

void rainbowPattern(Adafruit_NeoPXL8* leds, StripCount* stripCount, StripInformation* stripInformation, uint32_t millis, bool reversed) {
    if(stripInformation->count == 0) return;
    double speedMultiplier = double((millis % stripInformation->speed)) / double(stripInformation->speed);

    for (int i = stripCount->startIndex; i < stripCount->endIndex; i++) {
        int actualI = i - stripCount->startIndex;
        int actualCount = stripCount->endIndex - stripCount->startIndex - 1;

        int patternNdex = 0;
        if(!reversed) {
          patternNdex = Wrap(int((65536 * speedMultiplier) + (actualI * 65536.0 / actualCount)), 0, 65536);
        } else {
          patternNdex = Wrap(int((65536 * speedMultiplier) + (-actualI * 65536.0 / actualCount)), 0, 65536);
        }

        uint32_t rgbcolor = leds->gamma32(leds->ColorHSV(patternNdex));
        leds->setPixelColor(i, rgbcolor);
    }
}