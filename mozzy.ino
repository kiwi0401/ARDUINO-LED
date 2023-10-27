#include <Adafruit_NeoPXL8.h>
#include "LEDControl.h"
#include "RainbowPattern.h"

int8_t pins[8] = { RX, TX, MISO, 13, 5, SDA, A4, A3 };
Adafruit_NeoPXL8 leds(NUM_LEDS, pins, COLOR_ORDER);

void setup() {
  if (!leds.begin()) {
    pinMode(LED_BUILTIN, OUTPUT);
    for (;;) digitalWrite(LED_BUILTIN, (millis() / 500) & 1);
  }

  leds.setBrightness(100);  // Tone it down, NeoPixels are BRIGHT!
  for (uint32_t color = 0xFF0000; color > 0; color >>= 8) {
    leds.fill(color);
    leds.show();
    delay(500);
  }

  leds.fill(0);
}

StripCount getCount(StripInformation* strip) {
  if(strip->index == -1) return StripCount(0,0);
  return StripCount(strip->index * NUM_LEDS, strip->index * NUM_LEDS + strip->count);
}

void doEarControl(Adafruit_NeoPXL8* leds, uint32_t now) {
  StripCount ear_counts = getCount(&EARS);
  StripCount left_ear_count = StripCount(ear_counts.startIndex, ear_counts.startIndex + ((ear_counts.endIndex - ear_counts.startIndex) / 2) - 1);
  StripCount right_ear_count = StripCount(ear_counts.startIndex + ((ear_counts.endIndex - ear_counts.startIndex) / 2), ear_counts.endIndex);

  rainbowPattern(leds, &left_ear_count, &EARS, now, true);
  rainbowPattern(leds, &right_ear_count, &EARS, now, false);

  StripCount ear_counts_left = getCount(&EARS_LEFT);
  StripCount ear_counts_right = getCount(&EARS_RIGHT);

  rainbowPattern(leds, &ear_counts_left, &EARS_LEFT, now, true);
  rainbowPattern(leds, &ear_counts_right, &EARS_RIGHT, now, true);
}

void loop() {
  uint32_t now = millis();  // Get time once at start of each frame
  uint32_t testColor = leds.Color(leds.gamma8(255), leds.gamma8(0), leds.gamma8(0));

  StripCount eye_left_counts = getCount(&EYE_LEFT);
  rainbowPattern(&leds, &eye_left_counts, &EYE_LEFT, now, true);

  StripCount eye_right_counts = getCount(&EYE_RIGHT);
  rainbowPattern(&leds, &eye_right_counts, &EYE_RIGHT, now, false);

  StripCount jaw_up_counts = getCount(&JAW_UP);
  rainbowPattern(&leds, &jaw_up_counts, &JAW_UP, now, true);

  StripCount jaw_low_counts = getCount(&JAW_LOW);
  rainbowPattern(&leds, &jaw_low_counts, &JAW_LOW, now, true);

  uint32_t cheek_color = leds.Color(leds.gamma8(255), leds.gamma8(0), leds.gamma8(255));
  StripCount cheek_right_counts = getCount(&CHEEK_RIGHT);
  if(CHEEK_RIGHT.count != 0) leds.fill(cheek_color, cheek_right_counts.startIndex, CHEEK_RIGHT.count);

  StripCount cheek_left_counts = getCount(&CHEEK_LEFT);
  if(CHEEK_LEFT.count != 0) leds.fill(cheek_color, cheek_left_counts.startIndex, CHEEK_LEFT.count);

  doEarControl(&leds, now);

  leds.show();
}