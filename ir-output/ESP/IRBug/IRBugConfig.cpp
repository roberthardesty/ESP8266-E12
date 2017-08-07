#include "IRBugConfig.h"

//These two methods have been extracted to make changing the properties easier
void IRBugConfig::extractProperties(JsonObject& root){
  wifiFailCount = root["wifiFailCount"];
  configMode = root["configMode"];
  transmitterMode = root["transmitterMode"];
  irCode = root["irCode"].asString();
  duration = root["duration"];
  canSend = root["canSend"];
}
//These two methods have been extracted to make changing the properties easier
void IRBugConfig::addPropertiesToJson(JsonObject& root){
  root["wifiFailCount"] = wifiFailCount;
  root["configMode"] = configMode;
  root["transmitterMode"] = transmitterMode;
  root["irCode"] = irCode;
  root["duration"] = duration;
  root["canSend"] = canSend;
}


//loads/creates config and loads it into properties
bool IRBugConfig::init(){
  if(!SPIFFS.exists("/config.json")) createConfig();
  JsonObject& root = loadConfigJSON();
  if(!root.success()) return false;
  extractProperties(root);
  return true;
}

bool IRBugConfig::load(String httpGetString){
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parse(httpGetString);
  if(root.success()){
    Serial.println("Parsed Successfully");
    extractProperties(root["data"]["result"]);
    return true;
  }
  Serial.println("Parse FAIL");
  return false;
}

//save properties back to file
bool IRBugConfig::save(){
  StaticJsonBuffer<300> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  addPropertiesToJson(root);
  //check the json that should have been created here
  root.printTo(Serial);
 
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

JsonObject& IRBugConfig::loadConfigJSON(){
  SPIFFS.begin();  
  String configJsonString = "fail";
  {//RAM Optimization? Scope the file.
    File myConfigFile = SPIFFS.open("/config.json", "r");
    if(myConfigFile){
      configJsonString = myConfigFile.readString();
      myConfigFile.close();
    }
    SPIFFS.end();
  }
  //something tells me this might not work
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

