#ifndef LEDControl_h
#define LEDControl_h

#include <Arduino.h>
#include <Adafruit_NeoPXL8.h>
#include <Adafruit_NeoPixel.h>

#define NUM_LEDS    100
#define COLOR_ORDER NEO_GRB

#define BRIGHTNESS 100

struct Color {
  int red;
  int green;
  int blue;
};

struct StripInformation {
  int count;
  int speed;
  int index;
  StripInformation()
    : count(0), speed(0), index(0) {}
  StripInformation(int count, int speed, int index)
    : count(count), speed(speed), index(index) {}
};

struct StripCount {
  int startIndex;
  int endIndex;
  StripCount(int startIndex, int endIndex) : startIndex(startIndex), endIndex(endIndex) {}
};

extern  StripInformation EYE_LEFT;
extern  StripInformation EYE_RIGHT;
extern  StripInformation JAW_UP;
extern  StripInformation JAW_LOW;
extern  StripInformation CHEEK_RIGHT;
extern  StripInformation CHEEK_LEFT;
extern  StripInformation EARS;
extern StripInformation EARS_LEFT;
extern StripInformation EARS_RIGHT;

enum Pattern {Rainbow = 0, Off = 1, Hypno = 2, PinkPurple = 3, Explosive = 4, BoringAltyBlue = 5, TrailingExplosive = 6, Raindrop = 7, Chasing = 8}; 

int Wrap(int kX, int const kLowerBound, int const kUpperBound);

void LEDSetup();
void LEDLoop();

#endif
