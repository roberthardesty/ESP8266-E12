/*
 * IRremoteESP8266: IRrecvDemo - demonstrates receiving IR codes with IRrecv
 * An IR detector/demodulator must be connected to the input RECV_PIN.
 * Version 0.1 Sept, 2015
 * Based on Ken Shirriff's IrsendDemo
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff, http://arcfn.com
 */
#include <Arduino.h>
#include <IRrecv.h>
#include "FS.h"

//Setting for clearing the file during setup
bool resetFile = false;
// an IR detector/demodulator is connected to GPIO pin 10
uint16_t RECV_PIN = 2;
IRrecv irrecv(RECV_PIN);

decode_results results;

void setup() {
    Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
  Serial.println(); Serial.println(); Serial.println();
  for(uint8_t t = 4; t > 0; t--) {
      Serial.printf("[SETUP] WAIT %d...\n", t);
      Serial.flush();
      delay(1000);
  }
  Serial.println("Hello, we are setting things up for you");
  SPIFFS.begin();
  if(SPIFFS.exists("/ircodes.txt")){ 
    if(resetFile) SPIFFS.remove("/irocdes.txt");
    else{
       // open file for reading
      File f = SPIFFS.open("/ircodes.txt", "a+");
      if (!f) {
        Serial.println("file open failed");
      }  
      Serial.println("====== Reading from SPIFFS file =======");
      Serial.println("Reading IR code hex from file...");
      String s = f.readString();
      Serial.println(s);
      f.close();
    }
  }
  /* 
   * SPIFFS.format();
   * Serial.println("Formatting done");
  */
  Serial.println("Starting Receiver");
  irrecv.enableIRIn();  // Start the receiver
}
long currentTime = 0;
void loop() {
  if (irrecv.decode(&results)) {
    // print() & println() can't handle printing long longs. (uint64_t)
    // So we have to print the top and bottom halves separately.
    if (results.value >> 32)
      Serial.print((uint32_t) (results.value >> 32), HEX);
    Serial.println((uint32_t) (results.value & 0xFFFFFFFF), HEX);

    File f = SPIFFS.open("/ircodes.txt", "a+");
    f.println((uint32_t) (results.value), HEX);
    f.close();
    irrecv.resume();  // Receive the next value
  }
  delay(100);
}
