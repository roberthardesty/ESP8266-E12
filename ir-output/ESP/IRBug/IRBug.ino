
#include <ESP8266WiFi.h>
#include "IRBugConfig.h"
#include "IRBugTransmitter.h"
#include "HttpComm.h"

const unsigned long setupLength = 120000;
const unsigned long httpDelayLength = 10000;
const string wifiSSID = "davidjohnson";
const string wifiPASS = "ytsedrah";


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
  ESP.deepSleep(setupLength);
}

void startWifi(){
  int attemptCount = 0;
  WiFi.mode(WIFI_STA);
  if(WiFi.status() != WL_CONNECTED){
    WiFi.begin(wifiSSID, wifiSSID);
    Serial.print("Reconnecting...");
    attemptCount++;
  }
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(WiFi.status());
    WiFi.begin(wifiSSID, wifiPASS);
    if(attemptCount > 12){ configFile.wifiFailCount++; return; }//try 13 times then log failure and return
    attemptCount++;
  }
  configFile.wifiFailCount = 0;//if we exit the loop we connected successfully
}

bool attemptLoadConfig(){
  HttpComm httpComm;
  String res = httpComm.defaultGet();
  if(res == "") return false;

  return configFile.load(res);
}

uint32_t calculateSleepTime(){
  if(!configFile.canSend) return;
  ESP.deepSleep(configFile.duration * 60000);
}

typedef void (* transmitterFunct)();
transmitterFunct sendCode = [](){
  send_ir_codes(configFile.irCode);
};
transmitterFunct sleepTime = []() { 
  ESP.deepSleep(configFile.duration);
};

transmitterFunct tFuncts[2] = {sendCode, sleepTime};

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
    Serial.flush();
    uint32_t wifiFailSleepTime = 300000 * configFile.wifiFailCount; //hopefully failcount is zero otherwise 5 mins per fail.
    configFile.save();
    ESP.deepSleep(wifiFailSleepTime);
    //might want to make a double switch case method to load global booleans
a
    int attemptCount = 0;
    while(!attemptLoadConfig() && attemptCount < 5){attemptCount++;}

    if(configFile.configMode == 2) runSetupCycle();
    else if(configFile.configMode == 1) ESP.deepSleep(configFile.duration);
    //If we make it this far we are in transmit mode;
    if(configFile.canSend) send_ir_codes(configFile.irCode);
    configFile.canSend = configFile.transmitterMode == 2;
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
