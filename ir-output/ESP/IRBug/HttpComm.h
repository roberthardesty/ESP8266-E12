#ifndef HttpComm_h
#define HttpComm_h
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define NODE_RED_HOME_HOST "192.168.1.68"
#define NODE_RED_HOME_DEFAULT_ENDPOINT "/irbug"
#define NODE_RED_PORT 3000

class HttpComm {
  public: 
    String defaultGet();
    void setupGet();
  private: 
    HTTPClient http;
};
#endif

