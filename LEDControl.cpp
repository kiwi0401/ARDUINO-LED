#include "LEDControl.h"

Adafruit_NeoPixel cheek_right(CHEEK_RIGHT, Dig2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel eye_left(EYE_LEFT, Dig3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel cheek_left(CHEEK_LEFT, Dig5, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel eye_right(EYE_RIGHT, Dig4, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel jaw_upper(JAW_UP, Dig7, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel jaw_lower(JAW_LOW, Dig6, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel ears(EARS, Dig8, NEO_GRB + NEO_KHZ800);

void LEDSetup() {
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif

  eye_left.begin();
  eye_left.setBrightness(BRIGHTNESS);

  eye_right.begin();
  eye_right.setBrightness(BRIGHTNESS);

  jaw_upper.begin();
  jaw_upper.setBrightness(100);

  jaw_lower.begin();
  jaw_lower.setBrightness(100);

  cheek_right.begin();
  cheek_right.setBrightness(100);

  cheek_left.begin();
  cheek_left.setBrightness(100);

  ears.begin();
  ears.setBrightness(100);
}

int eyesBrightness = 20;
int earsBrightness = 100;
int jawsBrightness = 100;
int cheeksBrightness = 100;

String earsPattern = "Rainbow";
String eyesPattern = "Rainbow";
String jawsPattern = "Rainbow";
String cheeksPattern = "Rainbow";

int eyesSpeed = 1;
int earsSpeed = 1;
int jawsSpeed = 1;
int cheeksSpeed = 1;

Color eyesColor = {0, 0, 255};
Color earsColor = {0, 0, 255};
Color jawsColor = {0, 0, 255};
Color cheeksColor = {0, 0, 255};

void LEDLoop() {
  
}

void toggleStrip(String strip) {
  if(strcmp(strip, "EYES")) toggleEyes = true;
  if(strcmp(strip, "EYES")) toggleEyes = true;


  
  EYES,EARS,MOUTH,CHEEKS
}
