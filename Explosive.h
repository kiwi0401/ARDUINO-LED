// Ensure the header isn't included more than once.
#ifndef EXPLOSIVE_H
#define EXPLOSIVE_H

#include "LEDControl.h"

struct ExplosiveParticle {
  double position;
  double speed;
  double acceleration;
  double hue;
  double brightness;
  double lifespan;

  ExplosiveParticle()
    : position(0), speed(0), acceleration(0), hue(0), brightness(0), lifespan(0) {}
  ExplosiveParticle(double position, double speed, double acceleration, double hue, double brightness, double lifespan)
    : position(position), speed(speed), acceleration(acceleration), hue(hue), brightness(brightness), lifespan(lifespan) {}
};

class LedStrip {
private:
  double modificationPattern(double x, int stripCount);
  void handleDisplayParticle(ExplosiveParticle particle, Adafruit_NeoPXL8* leds, StripInformation* stripInformation, StripCount* stripCount, bool isExplosion);
  void doExplosion(Adafruit_NeoPXL8* leds, StripInformation* stripInformation, StripCount* stripCount);
  ExplosiveParticle explosiveParticles[10];
  int numParticles;
  int prevMillis;
  bool exploded;
  bool started;
  double explodedPosition;
  int brightness;
  int explosionTime;

public:
  // Constructor
  LedStrip();
  LedStrip(int brightness);


  // Member function to apply the explosive pattern
  void explosivePattern(Adafruit_NeoPXL8* leds, StripCount* stripCount, StripInformation* stripInformation, uint32_t millis, bool reversed);
};

#endif  // EXPLOSIVE_H
