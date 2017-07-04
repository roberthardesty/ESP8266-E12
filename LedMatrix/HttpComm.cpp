#include "HttpComm.h"

String HttpComm::defaultGET(){
  http.begin(NODE_RED_HOME_HOST); //HTTPS
  int httpCode = http.GET();
  // httpCode will be negative on error
  if(httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] GET... code: %d\n", httpCode); 
      // file found at server
  } else {
      Serial.printf("[HTTP] GET... failed, error: ");
  }
  if(httpCode == 200) {
    Serial.println("About to get String");
    String payload = http.getString();; 
    long payloadSize = sizeof(payload);
    Serial.println(payload);
    Serial.println("-----------------------------------------------\n");
    Serial.println("Size: " + (String)payloadSize);
    return payload;
    
  }
  http.end();
  return "";
}
/*  
    JsonObject& root = jsonBuffer.parseObject(headLineString);
    if(!root.success())
    Serial.println("Failed to Parse");
  else{
    Serial.println("Parsing Success!");
    const char* test = root["source"];
    Serial.println(test);    
*/
