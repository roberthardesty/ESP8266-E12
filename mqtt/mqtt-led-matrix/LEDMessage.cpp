#include "LEDMessage.h"

//These two methods have been extracted to make changing the properties easier
void LEDMessage::extractProperties(JsonObject& root){
  messageString = root["message"];
  //wifiFailCount = root["wifiFailCount"];
  ledMatrixMode = root["ledMatrixMode"];
}
//These two methods have been extracted to make changing the properties easier
void LEDMessage::addPropertiesToJson(JsonObject& root){
  //root["wifiFailCount"] = wifiFailCount;
  root["messageString"] = messageString;
  root["ledMatrixMode"] = ledMatrixMode;
}


//loads/creates config and loads it into properties
bool LEDMessage::init(){
  if(!SPIFFS.exists("/config.json")) createConfig();
  JsonObject& root = loadConfigJSON();
  if(!root.success()) return false;
  extractProperties(root);
  return true;
}

bool LEDMessage::load(String jsonString){
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parse(jsonString);
  if(root.success()){
    Serial.println("Parsed Successfully");
    extractProperties(root["data"]["ledObject"]);
    return true;
  }
  Serial.println("Parse FAIL");
  return false;
}

//save properties back to file
bool LEDMessage::save(){
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

JsonObject& LEDMessage::loadConfigJSON(){
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

void LEDMessage::createConfig(){
  SPIFFS.begin();  
  File myConfigFile = SPIFFS.open("/config.json", "w");
  if(myConfigFile){
    myConfigFile.print("{\"wifiFailCount\":0, \"messageString\": \"Config Created\",\"ledMatrixMode\":0}");
    myConfigFile.close();
  }
  SPIFFS.end();
}

