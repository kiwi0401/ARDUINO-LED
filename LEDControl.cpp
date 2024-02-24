#include "LEDControl.h"
#include "RainbowPattern.h"

//MOZZY
StripInformation EYE_LEFT = StripInformation(28, 1000, 0, 100);
StripInformation EYE_RIGHT = StripInformation(28, 1000, 5, 100);
StripInformation JAW_UP = StripInformation(28, 1000, 3, 100);
StripInformation JAW_LOW = StripInformation(33, 1000, 4, 100);
StripInformation CHEEK_RIGHT = StripInformation(3, 1000, 1, 100);
StripInformation CHEEK_LEFT = StripInformation(3, 1000, 6, 100);
StripInformation EARS = StripInformation(41, 1000, 2, 100);
StripInformation EARS_LEFT = StripInformation(0, 1000, -1);
StripInformation EARS_RIGHT = StripInformation(0, 1000, -1);

//ALTY
// StripInformation EYE_LEFT = StripInformation(25, 1000, 2, 150);
// StripInformation EYE_RIGHT = StripInformation(25, 1000, 4, 150);
// StripInformation JAW_LOW = StripInformation(21, 1000, 1);

// StripInformation EARS_LEFT = StripInformation(43, 1000, 3);
// StripInformation EARS_RIGHT = StripInformation(42, 1000, 7);

// StripInformation JAW_UP = StripInformation(0, 1000, -1);
// StripInformation CHEEK_RIGHT = StripInformation(0, 1000, -1);
// StripInformation CHEEK_LEFT = StripInformation(0, 1000, -1);
// StripInformation EARS = StripInformation(0, 1000, -1);

int Wrap(int kX, int const kLowerBound, int const kUpperBound) {
  int range_size = kUpperBound - kLowerBound + 1;

  if (kX < kLowerBound)
    kX += range_size * ((kLowerBound - kX) / range_size + 1);

  return kLowerBound + (kX - kLowerBound) % range_size;
}

LedController::LedController(StripInformation stripInformation, StripCount stripCounts, Adafruit_NeoPXL8* leds)
  : leds(leds), numStrips(1) {
  this->stripCounts = (StripCount*)malloc(sizeof(StripCount));
  this->stripCounts[0] = stripCounts;
  this->stripInformation = (StripInformation*)malloc(sizeof(StripInformation));
  this->stripInformation[0] = stripInformation;
  LedController::pattern = Rainbow;
}

LedController::LedController(StripInformation *stripInformation, StripCount* stripCounts, Adafruit_NeoPXL8* leds, int numStrips)
  : stripInformation(stripInformation), stripCounts(stripCounts), leds(leds), numStrips(numStrips) {
  LedController::pattern = Rainbow;
}

void LedController::runPattern() {
  uint32_t now = millis();
  for (int i = 0; i < this->numStrips; i++) {
    StripCount stripCount = this->stripCounts[i];
    StripInformation stripInformation = this->stripInformation[i];
    switch (LedController::pattern) {
      case Rainbow:
        {
          rainbowPattern(this->leds, &stripCount, &stripInformation, now, true);
          break;
        }
      default:
        {
          for (int i = stripCount.startIndex; i < stripCount.endIndex; i++) this->leds->setPixelColor(i, 0);
        }
    }
  }
}