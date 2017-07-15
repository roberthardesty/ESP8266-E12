#include "HttpComm.h"

String HttpComm::defaultGet(){
  http.begin(NODE_RED_HOME_HOST, NODE_RED_PORT, NODE_RED_HOME_DEFAULT_ENDPOINT, ""); //HTTPS
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
    String payload = http.getString();
    Serial.println(payload);
    Serial.println("-----------------------------------------------\n");
    return payload;
    
  }
  http.end();
  return "";
}

void HttpComm::setupGet(){
  http.begin(NODE_RED_HOME_HOST, NODE_RED_PORT, NODE_RED_HOME_DEFAULT_ENDPOINT, "");
  int httpCode = http.GET();
  http.end();
}


