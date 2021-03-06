#include "HttpComm.h"

void HttpComm::defaultPOST(String json){
  http.begin(NODE_RED_HOME_HOST, NODE_RED_PORT, NODE_RED_HOME_DEFAULT_ENDPOINT, false); //HTTP
  http.addHeader("Content-Type", "text/plain");
  int httpCode = http.POST(json);
  
  // httpCode will be negative on error
  if(httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] POST... code: %d\n", httpCode);
  
      // file found at server
      if(httpCode == 200) {
          String payload = http.getString();
          Serial.println("Sucessfully delivered: " + payload);
          Serial.println("-----------------------------------------------\n");
      }
  } else {
      Serial.printf("[HTTP] POST... failed, error: ");
  }
  
  http.end();
}

