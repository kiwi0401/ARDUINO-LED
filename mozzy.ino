#include <Adafruit_NeoPXL8.h>
#include "LEDControl.h"
#include "RainbowPattern.h"
#include "Explosive.h"
#include <WiFi.h>
#include <ArduinoOTA.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <String.h>

void doLeds();
void doEarControl(Adafruit_NeoPXL8* leds, uint32_t now);
void handleCommand(String command);
Pattern getPatternFromString(String pattern);
LedController* getControllerFromString(String msg);

BLECharacteristic* pCharacteristic;

int8_t pins[8] = { RX, TX, MISO, 13, 5, SDA, A4, A3 };
Adafruit_NeoPXL8 leds(NUM_LEDS, pins, COLOR_ORDER);

#define SERVICE_UUID "12345678-1234-1234-1234-123456789012"
#define CHARACTERISTIC_UUID "87654321-4321-4321-4321-210987654321"

LedController* EYE_LEFT_CONTROLLER;
LedController* EYE_RIGHT_CONTROLLER;
LedController* EAR_LEFT_CONTROLLER;
LedController* EAR_RIGHT_CONTROLLER;
LedController* MOUTH_UP_CONTROLLER;
LedController* MOUTH_LOW_CONTROLLER;
LedController* CHEEKS_CONTROLLER;

class MessageReciever : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic* pCharacteristic) {
    std::string value = pCharacteristic->getValue();
    if (value.length() > 0) {
      Serial.println("Received via BLE: ");
      String command = "";
      for (int i = 0; i < value.length(); i++) {
        command += (char)value[i];
      }
      Serial.println(command);
      handleCommand(command);
    }
  }
};

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    Serial.println("Client connected");
  }

  void onDisconnect(BLEServer* pServer) {
    Serial.println("Client disconnected");
    BLEDevice::startAdvertising();
    // You can add additional cleanup or reconnection logic here
  }
};

void doBLESetup() {
  BLEDevice::init("Mozzy-Head-LED");
  BLEServer* pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  BLEService* pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);

  pService->start();
  BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);

  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  BLEDevice::startAdvertising();

  pCharacteristic->setCallbacks(new MessageReciever());

  Serial.println("BLE server is ready");
}

void doWifiSetup() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP("Mozzy-Brained", "Gloop1124441");

  delay(500);

  ArduinoOTA.begin();

  Serial.println("Access Point Started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());
}

void doLedSetup() {
  if (!leds.begin()) {
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.println("DIDNT WORK");
    for (;;) digitalWrite(LED_BUILTIN, (millis() / 500) & 1);
  }

  leds.setBrightness(100);  // Tone it down, NeoPixels are BRIGHT!
  for (uint32_t color = 0xFF0000; color > 0; color >>= 8) {
    leds.fill(color);
    leds.show();
    delay(500);
  }

  // STRIP COUNTS
  StripCount eye_left_counts = getCount(&EYE_LEFT);
  StripCount eye_right_counts = getCount(&EYE_RIGHT);
  StripCount jaw_up_counts = getCount(&JAW_UP);
  StripCount jaw_low_counts = getCount(&JAW_LOW);
  StripCount cheek_right_counts = getCount(&CHEEK_RIGHT);
  StripCount cheek_left_counts = getCount(&CHEEK_LEFT);
  StripCount ear_counts = getCount(&EARS);

  StripCount* cheekCounts = (StripCount*)malloc(2 * sizeof(StripCount));
  StripInformation* cheekInformation = (StripInformation*)malloc(2 * sizeof(StripInformation));
  cheekCounts[0] = cheek_left_counts;
  cheekCounts[1] = cheek_right_counts;
  cheekInformation[0] = CHEEK_RIGHT;
  cheekInformation[1] = CHEEK_LEFT;

  StripCount left_ear_count = StripCount(ear_counts.startIndex, ear_counts.endIndex - 4);
  StripCount right_ear_count = StripCount(ear_counts.endIndex - 3, ear_counts.startIndex + 82);

  StripInformation ear_left_actual = EARS_LEFT.index == -1 ? EARS : EARS_LEFT;
  StripInformation ear_right_actual = EARS_RIGHT.index == -1 ? EARS : EARS_RIGHT;

  EYE_LEFT_CONTROLLER = new LedController(EYE_LEFT, eye_left_counts, &leds);
  EYE_RIGHT_CONTROLLER = new LedController(EYE_RIGHT, eye_right_counts, &leds);
  EAR_LEFT_CONTROLLER = new LedController(ear_left_actual, left_ear_count, &leds);
  EAR_RIGHT_CONTROLLER = new LedController(ear_right_actual, right_ear_count, &leds);
  MOUTH_UP_CONTROLLER = new LedController(JAW_UP, jaw_up_counts, &leds);
  MOUTH_LOW_CONTROLLER = new LedController(JAW_LOW, jaw_low_counts, &leds);
  CHEEKS_CONTROLLER = new LedController(cheekInformation, cheekCounts, &leds, 2);

  leds.fill(0);
  leds.show();
}

void setup() {
  Serial.begin(115200);
  Serial.println("STARTING");
  randomSeed(analogRead(0));
  doWifiSetup();
  doBLESetup();
  doLedSetup();
}

StripCount getCount(StripInformation* strip) {
  if (strip->index == -1) return StripCount(0, 0);
  return StripCount(strip->index * NUM_LEDS, strip->index * NUM_LEDS + strip->count);
}

void loop() {
  // Serial.println("RUNNING");
  ArduinoOTA.handle();
  doLeds();
}

void handleCommand(String command) {
  // Split the command string by ':'
  int colonIndex = command.indexOf(':');
  String password = command.substring(0, colonIndex);

  if (!password.equals(PASSWORD)) return;

  String data = command.substring(colonIndex + 1);

  // Split the data string by ';'
  int semiColonIndex;
  while ((semiColonIndex = data.indexOf(';')) != -1) {
    String segment = data.substring(0, semiColonIndex);
    data = data.substring(semiColonIndex + 1);

    // Split the segment by ','
    int commaIndex;
    String values[7];  // Maximum of 7 values (including strip identifier)
    int i = 0;
    while ((commaIndex = segment.indexOf(',')) != -1 && i < 7) {
      values[i++] = segment.substring(0, commaIndex);
      segment = segment.substring(commaIndex + 1);
    }
    values[i] = segment;  // Last value

    if(i <= 1) return;

    LedController* controller = getControllerFromString(values[0]);
    Pattern p = getPatternFromString(values[1]);

    

    if(controller != nullptr) {
      controller->pattern = p;
    }
  }
}

Pattern getPatternFromString(String msg) {
  msg.trim();
  if (msg.equals("Rainbow")) return Rainbow;
  else if (msg.equals("Off")) return Off;
  else if (msg.equals("Hypno")) return Hypno;
  else if (msg.equals("PinkPurple")) return PinkPurple;
  else if (msg.equals("Explosive")) return Explosive;
  else if (msg.equals("BoringAltyBlue")) return BoringAltyBlue;
  else if (msg.equals("TrailingExplosive")) return TrailingExplosive;
  else if (msg.equals("Raindrop")) return Raindrop;
  else if (msg.equals("Chasing")) return Chasing;
  
  return Off;
}

LedController* getControllerFromString(String msg) {
  msg.trim();
  if (msg.equals("EYE_LEFT")) return EYE_LEFT_CONTROLLER;
  else if (msg.equals("EYE_RIGHT")) return EYE_RIGHT_CONTROLLER;
  else if (msg.equals("EAR_LEFT")) return EAR_LEFT_CONTROLLER;
  else if (msg.equals("EAR_RIGHT")) return EAR_RIGHT_CONTROLLER;
  else if (msg.equals("MOUTH_UP")) return MOUTH_UP_CONTROLLER;
  else if (msg.equals("MOUTH_LOW")) return MOUTH_LOW_CONTROLLER;
  else if (msg.equals("CHEEKS")) return CHEEKS_CONTROLLER;

  return nullptr; // Return nullptr if no match is found
}


void doLeds() {
  EYE_LEFT_CONTROLLER->runPattern();
  EYE_RIGHT_CONTROLLER->runPattern();
  EAR_LEFT_CONTROLLER->runPattern();
  EAR_RIGHT_CONTROLLER->runPattern();
  MOUTH_UP_CONTROLLER->runPattern();
  MOUTH_LOW_CONTROLLER->runPattern();
  CHEEKS_CONTROLLER->runPattern();
  leds.show();
}