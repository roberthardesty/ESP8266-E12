#ifndef HttpComm_h
#define HttpComm_h
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>


#define NODE_RED_HOME_HOST "192.168.1.68"
#define NODE_RED_HOME_DEFAULT_ENDPOINT "/test"
#define NODE_RED_PORT 1800

class HttpComm {
  public: 
    void defaultPOST(String json);

  private: 
    HTTPClient http;
};
#endif

