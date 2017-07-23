
#include <ESP8266WiFi.h>
#include "IRBugConfig.h"
#include "IRBugTransmitter.h"
#include "HttpComm.h"

const unsigned long setupLength = 120000;
const unsigned long httpDelayLength = 10000;

IRBugConfig configFile;

void runSetupCycle(){
  HttpComm httpComm;
  bool isStillSetup = true;
  
  unsigned long startTime = millis();
  while(isStillSetup){
    send_single_ir_code(VOLUME_MUTE);
    unsigned long runTime = (millis() - startTime);
    isStillSetup = (runTime < setupLength);
    if(runTime % httpDelayLength == 0) httpComm.setupGet(); 
    delay(3000);
  }
}

void startWifi(){
  int attemptCount = 0;
  WiFi.mode(WIFI_STA);
  if(WiFi.status() != WL_CONNECTED){
    WiFi.begin("davidjohnson", "ytsedrah");
    Serial.print("Reconnecting...");
    attemptCount++;
  }
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(WiFi.status());
    WiFi.begin("davidjohnson", "ytsedrah");
    if(attemptCount > 12){ configFile.wifiFailCount++; return; }//try 13 times then log failure and return
    attemptCount++;
  }
  configFile.wifiFailCount = 0;//if we exit the loop we connected successfully
}

uint32_t calculateSleepTime(){
  
}

void setup() {
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
  Serial.println(); Serial.println(); Serial.println();
  for(uint8_t t = 4; t > 0; t--) {
      Serial.printf("[SETUP] WAIT %d...\n", t);
      Serial.flush();
      delay(200);
  }
  if(configFile.init()){  
    //attempt to start wifi and sleep appropriate amount on fails
    startWifi();
    uint32_t wifiFailSleepTime = 300000 * configFile.wifiFailCount; //hopefully failcount is zero otherwise 5 mins per fail.
    //deepSleep(wifiFailSleepTime);
      
    //might want to make a double switch case method to load global booleans
    if(configFile.configMode == 0) runSetupCycle();
    
    //deepSleep(calculateSleepTime());
  }else
    Serial.print("File Failed To Load");//this should never happen and can be removed after testing
}
  //*load config()
  //*wifi setup()
  //*http request()
  //*json parse()
  //battery voltage code
  //*spiffs save()
  //calculate sleep time()
  //test test test test test test

void loop() {

}
