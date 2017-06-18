#include "HttpComm.h"

String HttpComm::defaultGET(){
  http.begin(NODE_RED_HOME_HOST); //HTTPS
  int httpCode = http.GET();
  String payload = "";
  // httpCode will be negative on error
  if(httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);
  
      // file found at server
      if(httpCode == 200) {
          payload = http.getString();
          Serial.println("Sucessfully Requested: " + payload);
          Serial.println("-----------------------------------------------\n");
          return payload;
      }
  } else {
      Serial.printf("[HTTP] GET... failed, error: ");
  }
  
  http.end();

  return payload;
}

