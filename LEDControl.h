#ifndef LEDControl_h
#define LEDControl_h

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define Dig2 2
#define Dig3 3
#define Dig4 4
#define Dig5 5
#define Dig6 6
#define Dig7 7
#define Dig8 8

#define EYE_LEFT 23
#define EYE_RIGHT 23
#define JAW_UP 28
#define JAW_LOW 33
#define CHEEK_RIGHT 15
#define CHEEK_LEFT 15
#define EARS 82

#define EYE_SPEED 10
#define JAW_UP_SPEED 12
#define JAW_LOW_SPEED 12
#define CHEEK_SPEED 10
#define EARS_SPEED 10

#define BRIGHTNESS 20

struct Color {
  int red;
  int green;
  int blue;
};

enum Pattern {Rainbow = "RAINBOW", Off = "OFF", Hypno = "HYPNO", PinkPurple = "PINKPURPLE", Explosive = "EXPLOSIVE", BoringAltyBlue = "BORINGBLUE", TrailingExplosive = "TRAILINGEXPLOSIVE", Raindrop = "RAINDROP", Chasing = "CHASING"}; 


void LEDSetup();
void LEDLoop();
void eyePattern();
void jawUpperPattern();
void jawLowerPattern();
void cheekPattern();
void earPattern();

#endif
