#ifndef IRBugConfig_h
#define IRBugConfig_h

#include <ArduinoJson.h>
#include <Arduino.h>
#include "FS.h"

class IRBugConfig {
  public: 
    byte wifiFailCount;
    String ghostCode;
    byte configMode;
    byte transmitterMode;
    bool isGhost;
    bool isSetupMode;
    bool init();
    bool save();
    void setGhostCode(byte hours, String code);
  private: 
    byte ghostHours;
    void createConfig();
    JsonObject& loadConfigJSON();
};



#endif
