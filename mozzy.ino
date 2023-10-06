#include <Adafruit_BLE.h>
#include <Adafruit_BluefruitLE_UART.h>
#include "BluefruitConfig.h"
#include "LEDControl.h"

SoftwareSerial bluefruitSS = SoftwareSerial(BLUEFRUIT_SWUART_TXD_PIN, BLUEFRUIT_SWUART_RXD_PIN);

Adafruit_BluefruitLE_UART ble(bluefruitSS, BLUEFRUIT_UART_MODE_PIN,
                              BLUEFRUIT_UART_CTS_PIN, BLUEFRUIT_UART_RTS_PIN);
#define FACTORYRESET_ENABLE 1
void handleCommand(String command);

void setup() {
  LEDSetup();

  while (!Serial)
    ;  // Required for Flora & Micro
  delay(2000);

  Serial.begin(115200);
  Serial.println(F("Adafruit Bluefruit AT Command Example"));
  Serial.println(F("-------------------------------------"));

  /* Initialise the module */
  Serial.print(F("Initialising the Bluefruit LE module: "));

  if (!ble.begin(VERBOSE_MODE)) {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }
  Serial.println(F("OK!"));

  if (FACTORYRESET_ENABLE) {
    /* Perform a factory reset to make sure everything is in a known state */
    Serial.println(F("Performing a factory reset: "));
    if (!ble.factoryReset()) {
      error(F("Couldn't factory reset"));
    }
  }

  /* Disable command echo from Bluefruit */
  ble.echo(false);
  ble.setMode(BLUEFRUIT_MODE_DATA);

  Serial.println("Requesting Bluefruit info:");
  /* Print Bluefruit information */
  ble.info();
}

void loop() {
  BLELoop();
  LEDLoop();
}

void BLELoop() {
  String line = "";

  if (ble.available()) {
    while (ble.available()) {
      char c = (char)ble.read();
      if (c == '\n') {
        Serial.println(line);
        handleCommand(line);  // Pass the received line to a command handler function
        line = "";
      } else {
        line += c;
      }
    }
  }
}

void handleCommand(String command) {
  // Split the command string into parts based on the comma delimiter
  int delimiterIndex1 = command.indexOf(',');
  int delimiterIndex2 = command.indexOf(',', delimiterIndex1 + 1);
  int delimiterIndex3 = command.indexOf(',', delimiterIndex2 + 1);
  int delimiterIndex4 = command.indexOf(',', delimiterIndex3 + 1);
  int spaceIndex1 = command.indexOf(' ', delimiterIndex4 + 1);
  int spaceIndex2 = command.lastIndexOf(' ');

  // Ensure all delimiters were found
  if (delimiterIndex1 > 0 && delimiterIndex2 > delimiterIndex1 && delimiterIndex3 > delimiterIndex2 
      && delimiterIndex4 > delimiterIndex3 && spaceIndex1 > delimiterIndex4 && spaceIndex2 > spaceIndex1) {
    String strip = command.substring(0, delimiterIndex1);
    String pattern = command.substring(delimiterIndex1 + 1, delimiterIndex2);
    String speed = command.substring(delimiterIndex2 + 1, delimiterIndex3);
    String brightness = command.substring(delimiterIndex3 + 1, delimiterIndex4);
    String redStr = command.substring(delimiterIndex4 + 1, spaceIndex1);
    String greenStr = command.substring(spaceIndex1 + 1, spaceIndex2);
    String blueStr = command.substring(spaceIndex2 + 1);

    // Convert the pattern, speed, brightness, red, green, and blue strings to integer values
    int patternValue = pattern.toInt();
    int speedValue = speed.toInt();
    int brightnessValue = brightness.toInt();
    int redValue = redStr.toInt();
    int greenValue = greenStr.toInt();
    int blueValue = blueStr.toInt();

    
  }
  else {
    Serial.println("Invalid command format");
  }
}




void error(const __FlashStringHelper* err) {
  Serial.println(err);
  while (1)
    ;
}
