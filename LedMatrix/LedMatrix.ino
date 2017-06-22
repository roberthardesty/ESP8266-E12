#include <ArduinoJson.h>
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
 DynamicJsonBuffer jsonBuffer;

void ledInit(){
  ledMatrix.init();
  ledMatrix.setIntensity(LED_INTENSITY);
  ledMatrix.setCharWidth(CHAR_WIDTH);// range is 0-15
  ledMatrix.setRotation(true);
}
void startWifi(){
  WiFi.mode(WIFI_STA);
  if(WiFi.status() != WL_CONNECTED){
    WiFi.begin("davidjohnson", "ytsedrah");
    Serial.print("Reconnecting...");
  }
      while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      Serial.print(WiFi.status());
      WiFi.begin("davidjohnson", "ytsedrah");
    }
}

void setup() {
  Serial.begin(115200); // For debugging output
  Serial.println(); Serial.println(); Serial.println();
  for(uint8_t t = 4; t > 0; t--) {
      Serial.printf("[SETUP] WAIT %d...\n", t);
      Serial.flush();
      delay(1000);
  }
  startWifi();
  ledInit();
  String headLineString = myHttp.defaultGET();
  while(headLineString == ""){
    headLineString = myHttp.defaultGET();
    delay(200);
  }   
  JsonObject& root = jsonBuffer.parse(headLineString);
  if(root.success()){
    Serial.println("Parsed Successfully");
  }
  else{
    Serial.println("Parse FAIL");
  }
  JsonArray& articles = root["articles"];
  for(JsonArray::iterator it=articles.begin(); it!=articles.end(); ++it) 
  {
    // it->value contains the JsonVariant which can be casted as usual
    JsonObject& articleObject = *it;
    const char* articleDescription = articleObject["description"];
    Serial.println(articleDescription);
  }
  ledMatrix.setText("DIRTY WORD! DIRTY WORD! DIRTY WORD! TGIF!");

}

void loop() {
  ledMatrix.clear();
  ledMatrix.scrollTextLeft();
  ledMatrix.drawText();
  ledMatrix.commit(); // commit transfers the byte buffer to the displays
  delay(50);
}
