#include <cp437font.h>
#include <LedMatrix.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "HttpComm.h"

#define NUMBER_OF_DEVICES 4
#define CS_PIN 2
#define LED_INTENSITY 4
#define CHAR_WIDTH 7

LedMatrix ledMatrix = LedMatrix(NUMBER_OF_DEVICES, CS_PIN);
HttpComm myHttp;

void ledInit(){
  ledMatrix.init();
  ledMatrix.setIntensity(LED_INTENSITY);
  ledMatrix.setCharWidth(CHAR_WIDTH);// range is 0-15
  ledMatrix.setRotation(true);
}

void setup() {
  Serial.begin(115200); // For debugging output
  Serial.println(); Serial.println(); Serial.println();
  for(uint8_t t = 4; t > 0; t--) {
      Serial.printf("[SETUP] WAIT %d...\n", t);
      Serial.flush();
      delay(1000);
  }
  ledInit();
  myHttp.defaultGET();
  ledMatrix.setText("DIRTY WORD! DIRTY WORD! DIRTY WORD! TGIF!");
}

void loop() {
  ledMatrix.clear();
  ledMatrix.scrollTextLeft();
  ledMatrix.drawText();
  ledMatrix.commit(); // commit transfers the byte buffer to the displays
  delay(50);
}
