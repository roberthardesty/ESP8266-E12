#include "IRBugConfig.h"

//loads/creates config and loads it into properties
bool IRBugConfig::init(){
   if(!SPIFFS.exists("/config.json")) createConfig();
  
  JsonObject& root = loadConfigJSON();
  if(!root.success()) return false;

  configMode = root["configMode"];
  transmitterMode = root["transmitterMode"];
  wifiFailCount = root["wifiFailCount"];
  ghostHours = root["ghostHours"];
  isGhost = ghostHours > 0;
  ghostCode = root["ghostCode"].asString();
  return true;
}

//save properties back to file
bool IRBugConfig::save(){
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  if(isGhost){//dont save isGhost because that is calculated
    root["ghostHours"] = ghostHours -1;
    root["ghostCode"] = ghostCode;
  }
  else{
    root["ghostHours"] = ghostHours;
    root["ghostCode"] = "";
  }
  
  root["configMode"] = configMode;
  root["transmitterMode"] = transmitterMode;
  root["wifiFailCount"] = wifiFailCount;

  bool wasSuccessful = false;
  SPIFFS.begin();  
  SPIFFS.remove("/config.json");
  File myConfigFile = SPIFFS.open("/config.json", "w");
  if(myConfigFile){
    root.printTo(myConfigFile);
    myConfigFile.close();
    wasSuccessful = true;
  }
  SPIFFS.end();
  
  return wasSuccessful;
}

void IRBugConfig::setGhostCode(byte hours, String code){
  isGhost = true;
  //save subtracts one to count down to ghost stop
  ghostHours = hours + 1;
  ghostCode = code;
}

JsonObject& IRBugConfig::loadConfigJSON(){
  SPIFFS.begin();  
  String configJsonString = "fail";
  File myConfigFile = SPIFFS.open("/config.json", "r");
  if(myConfigFile){
    configJsonString = myConfigFile.readString();
    myConfigFile.close();
  }
  SPIFFS.end();
  DynamicJsonBuffer jsonBuffer;
  return jsonBuffer.parse(configJsonString);
}

void IRBugConfig::createConfig(){
  SPIFFS.begin();  
  File myConfigFile = SPIFFS.open("/config.json", "w");
  if(myConfigFile){
    myConfigFile.print("{\"ghostHours\":0,\"ghostCode\":\"\",\"wifiFailCount\":0, \"configMode\": 1,\"transmitterMode\":0}");
    myConfigFile.close();
  }
  SPIFFS.end();
}

