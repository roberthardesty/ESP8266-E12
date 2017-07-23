#ifndef IRBugConfig_h
#define IRBugConfig_h

#include <ArduinoJson.h>
#include <Arduino.h>
#include "FS.h"

class IRBugConfig {

  public: 
    IRBugConfig(){
      init();
    };
    byte wifiFailCount;
    byte configMode;
    byte transmitterMode;
    String irCode;
    int duration;
    
    bool init();
    bool save();
    bool load(String jsonString);
    
  private:     
    void createConfig();
    void extractProperties(JsonObject& root);
    void addPropertiesToJson(JsonObject& root);
};



#endif
