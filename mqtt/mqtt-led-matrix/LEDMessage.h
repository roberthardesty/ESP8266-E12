#ifndef LEDMessage_h
#define LEDMessage_h

#include <ArduinoJson.h>
#include <Arduino.h>
#include "FS.h"

class LEDMessage {

  public: 
    LEDMessage(){
      //init();
    };
    byte wifiFailCount;
    byte status;
    String message;
    byte waitMinutes;
    
    bool init();
    String getString();
    bool load(String jsonString);
    
  private:     
    void createConfig();
    void extractProperties(JsonObject& root);
    void addPropertiesToJson(JsonObject& root);
};



#endif
