#include "Explosive.h"
#define EXPLOSION_TIME 800


double LedStrip::modificationPattern(double x, int stripCount) {
  double innerSin = (2 * x) / stripCount;

  return -stripCount * asin(innerSin * innerSin) + ((stripCount / 2.0) * M_PI);
  return 0.0;
}

void LedStrip::doExplosion(Adafruit_NeoPXL8* leds, StripInformation* stripInformation, StripCount* stripCount) {
  int millisDiff = millis() - LedStrip::explosionTime;
  double currentX = double(millisDiff / double(EXPLOSION_TIME)) * (stripInformation->count / 2.0);

  Serial.println("EXPLODING");
  Serial.println(currentX);
  Serial.println(LedStrip::explosionTime);
  Serial.println(millisDiff);

  double position = LedStrip::explodedPosition / 5000;
  double ledAdjustedPosition = fmod(LedStrip::explodedPosition, stripInformation->count);
  if (ledAdjustedPosition < 0) ledAdjustedPosition += stripInformation->count - 1;

  for (double i = 0; i < currentX; i++) {
    double ratioHue = LedStrip::modificationPattern(i, stripInformation->count) / ((stripInformation->count / 2.0) * M_PI);

    if(isnan(ratioHue)) ratioHue = 0.05;

    Serial.println("ratio");
    Serial.println(ratioHue);

    uint32_t color = leds->gamma32(leds->ColorHSV(65534 - (65534 * ratioHue - 10000), 255, LedStrip::brightness));
    int wrapped1 = Wrap(stripCount->startIndex + ledAdjustedPosition + i, stripCount->startIndex, stripCount->endIndex);
    int wrapped2 = Wrap(stripCount->startIndex + ledAdjustedPosition - i, stripCount->startIndex, stripCount->endIndex);
    leds->setPixelColor(wrapped1, color);
    leds->setPixelColor(wrapped2, color);
  }

  if (EXPLOSION_TIME < millisDiff) {
    LedStrip::exploded = false;
    LedStrip::started = false;
  }
}


void LedStrip::handleDisplayParticle(ExplosiveParticle particle, Adafruit_NeoPXL8* leds, StripInformation* stripInformation, StripCount* stripCount, bool isExplosion) {
  double extraFade = (abs(particle.speed) / 25.0) * (stripInformation->count / (81.0) );
  double position = (particle.position * stripInformation->count / 82.0) / 5000;

  double ledAdjustedPosition = fmod(position, stripInformation->count);
  if (ledAdjustedPosition < 0) ledAdjustedPosition += stripInformation->count - 1;

  double modifier = 255.0 / int(extraFade);
  double hueFade = 20000 / stripInformation->count;

  if (particle.speed > 0) {
    for (int i = int(ledAdjustedPosition); i > int(ledAdjustedPosition - extraFade); i--) {
      int wrapped = Wrap(stripCount->startIndex + i, stripCount->startIndex, stripCount->endIndex);
      uint32_t color = leds->gamma32(leds->ColorHSV(int(particle.hue + hueFade * i), 255, LedStrip::brightness));
      leds->setPixelColor(wrapped, color);
    }
  } else {
    for (int i = int(ledAdjustedPosition); i < int(ledAdjustedPosition + extraFade); i++) {
      int wrapped = Wrap(stripCount->startIndex + i, stripCount->startIndex, stripCount->endIndex);
      uint32_t color = leds->gamma32(leds->ColorHSV(int(particle.hue + hueFade * i), 255, LedStrip::brightness));
      leds->setPixelColor(wrapped, color);
    }
  }
}



LedStrip::LedStrip() : brightness(255) {}

LedStrip::LedStrip(int brightness) : brightness(brightness) {}

void LedStrip::explosivePattern(Adafruit_NeoPXL8* leds, StripCount* stripCount, StripInformation* stripInformation, uint32_t millis, bool reversed) {
  if (stripInformation->count == 0) return;
  int size = LedStrip::numParticles;
  if (size == 0 && !LedStrip::started) {
    Serial.println("Adding Particle");
    int reverse = reversed ? -1 : 1;
    LedStrip::explosiveParticles[size] = ExplosiveParticle(LedStrip::explodedPosition, reverse * 200, reverse * 0.27, 0, 100, 5000);
    LedStrip::numParticles++;
    size++;
    LedStrip::started = true;
  }

  for (int i = stripCount->startIndex; i < stripCount->endIndex; i++) leds->setPixelColor(i, leds->Color(0, 0, 0));

  ExplosiveParticle newExplosiveParticles[10];
  int newListSize = 0;
  for (int i = 0; i < size; i++) {
    ExplosiveParticle temp = LedStrip::explosiveParticles[i];
    LedStrip::handleDisplayParticle(temp, leds, stripInformation, stripCount, LedStrip::exploded);
    int millisDiff = millis - LedStrip::prevMillis;
    temp.position = temp.position + millisDiff * temp.speed;

    temp.speed = max(min(temp.speed + millisDiff * temp.acceleration, 1500.0), -1500.0);
    temp.lifespan -= millisDiff;
    LedStrip::explodedPosition = temp.position;
    if (temp.lifespan > 0) {
      newExplosiveParticles[newListSize] = (temp);
      newListSize++;
    } else if (LedStrip::started && temp.lifespan <= 0 && !LedStrip::exploded) {
      // Do Explosion
      LedStrip::exploded = true;
        Serial.println("SETTING EXPLOSION TIME");
        Serial.println(millis);

      LedStrip::explosionTime = millis;
    }
  }

  if (LedStrip::exploded) {
    LedStrip::doExplosion(leds, stripInformation, stripCount);
  }

  for (int i = 0; i < newListSize; i++) {
    LedStrip::explosiveParticles[i] = newExplosiveParticles[i];
  }
  LedStrip::numParticles = newListSize;
  LedStrip::prevMillis = millis;
}
