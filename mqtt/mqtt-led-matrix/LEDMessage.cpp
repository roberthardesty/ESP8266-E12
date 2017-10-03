#include "LEDMessage.h"
char messageBuffer[100];
//These two methods have been extracted to make changing the properties easier
void LEDMessage::extractProperties(JsonObject& root){
  message = root["Message"].as<String>();

  status = root["Status"];

  waitMinutes = root["WaitMinutes"];
}
//These two methods have been extracted to make changing the properties easier
void LEDMessage::addPropertiesToJson(JsonObject& root){
  root["Message"] = message;
  root["Status"] = status;
  root["WaitMinutes"] = waitMinutes;
}


//loads/creates config and loads it into properties
bool LEDMessage::init(){
  return true;
}

bool LEDMessage::load(String jsonString){
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.parse(jsonString);
  if(root.success()){
    Serial.println("Parsed Successfully");
    extractProperties(root);
    return true;
  }
  Serial.println("Parse FAIL");
  return false;
}

String LEDMessage::getString(){
  String jsonString;
  StaticJsonBuffer<100> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  addPropertiesToJson(root);
  //check the json that should have been created here
  root.printTo(jsonString);
  return jsonString;
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

